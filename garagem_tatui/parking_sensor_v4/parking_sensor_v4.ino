#include <ESP8266WiFi.h>    // Ou use #include <WiFi.h> se for ESP32
#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>

#define N_LEDS      16    // Ajuste para o número de LEDs
#define PIN         0
#define LED         2
#define trigPin     13
#define echoPin     14

// Definições de thresholds
int offThreshhold   = 200;
int closeThreshhold = 150;
int safeThreshhold  = 100;
int tooClose        =  50;
int stop            =  20;
int wait = 10 * 1000;        // Tempo de espera em milissegundos
float gap = 1.5;             // Threshold de ruído para detectar movimento

// Definindo cores
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t white = strip.Color(255, 255, 255);
uint32_t off = strip.Color(0, 0, 0);
uint32_t red = strip.Color(255, 5, 15);
uint32_t yellow = strip.Color(0, 255, 0);
uint32_t green = strip.Color(255, 255, 0);
uint32_t purple = strip.Color(255, 0, 0);

// Variáveis para o sensor ultrassônico
long duration;
float distancecm;
float oldDistance = 0;

unsigned long changeTime;
boolean noChange = false;
boolean sleepTime = false;
boolean even = true;
int middle = 0;
int range;
int adj = 1;
int adj2 = 0;

const char *ssid = "JPL |||";          // Substitua pelo seu SSID
const char *password = "1123@juarez";  // Substitua pela sua senha

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  middle = N_LEDS / 2;
  if (N_LEDS % 2 == 1) {
    even = false;
  }
  if (even == true) {
    adj = 1;
    adj2 = 0;
  } else {
    adj = 0;
    adj2 = 1;
  }
  
  Serial.begin(115200);
  strip.begin();

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Configuração OTA
  ArduinoOTA.setHostname("ESP8266_LED_parking_sensor");
  ArduinoOTA.setPassword("12345678");  // Defina sua senha aqui

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
      Serial.println("Falha de autenticação");
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

void loop() {
  ArduinoOTA.handle();  // Chama a função OTA dentro do loop

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distancecm = duration * .0342 / 2;
  Serial.println(distancecm);
  
  sleep();
}

void sleep() {
  if (distancecm >= oldDistance) {
    if (distancecm - oldDistance <= gap) {
      noChange = true;
    } else {
      noChange = false;
    }
  } else {
    if (oldDistance - distancecm <= gap) {
      noChange = true;
    } else {
      noChange = false;
    }
  }

  if (noChange == true) {
    if (millis() - changeTime > wait) {
      sleepTime = true;
    }
  } else {
    sleepTime = false;
  }

  if (distancecm > offThreshhold || sleepTime == true) {
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, off);
    }
    strip.show();
  } else {
    colorChange2();
    if (noChange == false) {
      changeTime = millis();
      oldDistance = distancecm;
    }
  }
}

void colorChange2() {
  if (distancecm <= offThreshhold && distancecm > closeThreshhold) {
    range = map(distancecm, offThreshhold, closeThreshhold, 0, middle + adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, off);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, red);
      strip.setPixelColor(middle - adj - i, red);
    }
  } else if (distancecm <= closeThreshhold && distancecm > safeThreshhold) {
    range = map(distancecm, closeThreshhold, safeThreshhold, 0, middle + adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, red);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, yellow);
      strip.setPixelColor(middle - adj - i, yellow);
    }
  } else if (distancecm <= safeThreshhold && distancecm > tooClose) {
    range = map(distancecm, safeThreshhold, tooClose, 0, middle + adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, yellow);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, green);
      strip.setPixelColor(middle - adj - i, green);
    }
  } else if (distancecm <= tooClose && distancecm > stop) {
    range = map(distancecm, tooClose, stop, 0, middle + adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, green);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, purple);
      strip.setPixelColor(middle - adj - i, purple);
    }
  } else if (distancecm < stop) {
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, white);
    }
    strip.show();
    delay(300);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, red);
    }
    strip.show();
    delay(300);
  }
  strip.show();
}

uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
