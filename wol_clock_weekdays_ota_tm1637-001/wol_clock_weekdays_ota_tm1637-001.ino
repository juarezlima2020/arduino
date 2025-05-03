//************* Wol Clock by Jon Fuge with Weekday LED strip ******************************

//************* Declare included libraries ******************************
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <TM1637Display.h> // Biblioteca para o display TM1637

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <ESPmDNS.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
#endif

//************* Declare structures ******************************
struct RGB {
  byte r, g, b;
};

struct TIME {
  byte Hour, Minute;
};

//************* Editable Options ******************************
const RGB Twelve     = { 150, 150, 150 };
const RGB Quarters   = { 164,   0,  40 };
const RGB Divisions  = {  32,   0,  20 };
const RGB Background = {   1,   3,  10 };
const RGB Hour       = { 255, 128,   0 };
const RGB Minute     = {   0, 200,   0 };
const RGB Second     = { 255,   0,   0 };

const RGB DayDefaultColor = {   0,  3, 0 };
const RGB TodayColor      = { 5, 0,  0 };

const char ClockGoBackwards = 1;
const TIME WeekNight      = {22, 30};
const TIME WeekMorning    = { 5, 15};
const TIME WeekendNight   = {23, 30};
const TIME WeekendMorning = { 5, 30};

const int day_brightness = 255;
const int night_brightness = 100;

const int hours_Offset_From_GMT = -3;
const char *ssid      = "JPL";
const char *password  = "123@juarez";

byte SetClock;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#define PIN            6 // Clock LED strip pin (use GPIO 2)
#define PIN_WEEKDAYS   7 // Weekdays LED strip pin

//************* TM1637 Display Pins ******************************
#define CLK 3  // Pino de Clock do TM1637
#define DIO 4  // Pino de Dados do TM1637

TM1637Display display(CLK, DIO); // Inicialização do display TM1637

//************* Declare NeoPixel ******************************
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel weekdayStrip = Adafruit_NeoPixel(7, PIN_WEEKDAYS, NEO_GRB + NEO_KHZ800);

//************* Setup function for Wol_Clock ******************************
void setup() {
  Serial.begin(115200); // Ative a porta Serial para depuração

  pixels.begin();
  pixels.clear();
  pixels.show();

  weekdayStrip.begin();
  weekdayStrip.setBrightness(day_brightness);
  weekdayStrip.clear();
  weekdayStrip.show();

  Draw_Clock(0, 1);

  WiFi.begin(ssid, password);
  Draw_Clock(0, 2);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Draw_Clock(0, 3);

  SetClockFromNTP();

  // Inicializa o Arduino OTA
  setupOTA();

  // Inicializa o display TM1637
  display.setBrightness(0x0f);  // Defina o brilho máximo do display
}

//************* OTA Setup Function ******************************
void setupOTA() {
  ArduinoOTA.setHostname("WolClock_007_ESP32_6_SIDE"); //relogio feito com caixa de cinto 6 lados 6ª versao
  ArduinoOTA.setPassword("12345678"); // Defina uma senha segura

  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Iniciando atualização OTA: " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nAtualização concluída.");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progresso OTA: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erro OTA [%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Falha na autenticação");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Falha ao iniciar");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Falha de conexão");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Falha ao receber");
    } else if (error == OTA_END_ERROR) {
      Serial.println("Falha ao finalizar");
    }
  });

  ArduinoOTA.begin();
}

//************* Function to Update TM1637 Display ******************************
void UpdateDisplay(time_t t) {
  // Renomeamos as variáveis para evitar conflito com funções da biblioteca TimeLib.h
  int currentHour = hour(t);
  int currentMinute = minute(t);
  
  // Atualize o display com o formato HH:MM
  display.showNumberDecEx(currentHour * 100 + currentMinute, 0x40, true, 4, 0); // 0x40 ativa os dois pontos entre HH e MM

  // Opcional: pause para sincronizar o segundo
  delay(1000 - (millis() % 1000));
}

//************* Main program loop for Wol_Clock ******************************
void loop() {
  ArduinoOTA.handle(); // Processa OTA
  time_t t = now();

  Draw_Clock(t, 4);
  Draw_WeekdayStrip(t);

  // Atualiza o display TM1637 com a hora e minuto
  UpdateDisplay(t);

  if (minute(t) == 0 && SetClock == 1) {
    SetClockFromNTP();
    SetClock = 0;
  } else if (minute(t) != 0) {
    delay(200);
    SetClock = 1;
  }
}

//************* Other functions for Wol_Clock ******************************
void SetClockFromNTP() {
  timeClient.update();
  setTime(timeClient.getEpochTime());
  adjustTime(hours_Offset_From_GMT * 3600);
  if (IsDst()) adjustTime(3600);
}

bool IsDst() {
  if (month() < 3 || month() > 10) return false;
  if (month() > 3 && month() < 10) return true;

  int previousSunday = day() - weekday();
  if (month() == 3) return previousSunday >= 24;
  if (month() == 10) return previousSunday < 24;
  return false;
}

void Draw_Clock(time_t t, byte Phase) {
  if (Phase == 0) {
    pixels.clear();
    pixels.show();
  }

  if (Phase >= 1) {
    for (int i = 0; i < 60; i++) {
      pixels.setPixelColor(i, pixels.Color(Background.r, Background.g, Background.b));
    }
  }

  if (Phase >= 2) {
    for (int i = 0; i < 60; i += 5) {
      pixels.setPixelColor(i, pixels.Color(Divisions.r, Divisions.g, Divisions.b));
    }
  }

  if (Phase >= 3) {
    for (int i = 0; i < 60; i += 15) {
      pixels.setPixelColor(ClockCorrect(i), pixels.Color(Quarters.r, Quarters.g, Quarters.b));
    }
    pixels.setPixelColor(ClockCorrect(0), pixels.Color(Twelve.r, Twelve.g, Twelve.b));
  }

  if (Phase >= 4) {
    pixels.setPixelColor(ClockCorrect(second(t)), pixels.Color(Second.r, Second.g, Second.b));
    pixels.setPixelColor(ClockCorrect(minute(t)), pixels.Color(Minute.r, Minute.g, Minute.b));
    pixels.setPixelColor(ClockCorrect((hour(t) % 12) * 5 + minute(t) / 12), pixels.Color(Hour.r, Hour.g, Hour.b));
  }

  SetBrightness(t);
  pixels.show();
}

void Draw_WeekdayStrip(time_t t) {
  for (int i = 0; i < 7; i++) {
    weekdayStrip.setPixelColor(i, weekdayStrip.Color(DayDefaultColor.r, DayDefaultColor.g, DayDefaultColor.b));
  }

  int today = weekday() - 1;
  weekdayStrip.setPixelColor(today, weekdayStrip.Color(TodayColor.r, TodayColor.g, TodayColor.b));
  weekdayStrip.show();
}

void SetBrightness(time_t t) {
  int NowHour = hour(t);
  int NowMinute = minute(t);
  if ((weekday() >= 2 && weekday() <= 6 && (NowHour > WeekNight.Hour || (NowHour == WeekNight.Hour &&
      NowMinute >= WeekNight.Minute) || (NowHour == WeekMorning.Hour && NowMinute <= WeekMorning.Minute) || 
      NowHour < WeekMorning.Hour)) ||
      ((NowHour > WeekendNight.Hour || (NowHour == WeekendNight.Hour && NowMinute >= WeekendNight.Minute) || 
      (NowHour == WeekendMorning.Hour && NowMinute <= WeekendMorning.Minute) || NowHour < WeekendMorning.Hour) && 
      (weekday() == 1 || weekday() == 7))) {
    pixels.setBrightness(night_brightness);
  } else {
    pixels.setBrightness(day_brightness);
  }
  pixels.show();
}

int ClockCorrect(int Pixel) {
  return (ClockGoBackwards == 1) ? ((60 - Pixel + 30) % 60) : Pixel;
}
