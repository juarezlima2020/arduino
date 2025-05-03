#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
#include <Servo.h>

// Configurações Wi-Fi
const char* ssid = "JPL"; // Nome da rede Wi-Fi
const char* password = "123@juarez"; // Senha da rede Wi-Fi

// Configuração do NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // Servidor NTP

// Pinos do NodeMCU
#define SERVO_PIN D1          // Pino do servo motor
#define BTN_TIME_PIN D2       // Botão para alternar tempo
#define BTN_MODE_PIN D3       // Botão para alternar modos
#define LED_GREEN_PIN D4      // LED verde (45 minutos)
#define LED_YELLOW_PIN D5     // LED amarelo (30 minutos)
#define LED_RED_PIN D6        // LED vermelho (15 minutos)
#define LED_BLUE_PIN D7       // LED azul (finais de semana)
#define LED_WHITE_PIN D8      // LED branco (todos os dias)

// Variáveis globais
Servo servo;
int timeInterval = 45 * 60 * 1000; // Intervalo padrão em milissegundos
bool weekendOnly = false; // Modo inicial (todos os dias)
unsigned long lastServoActivation = 0;
bool btnTimePressed = false;
bool btnModePressed = false;

void setup() {
  // Configuração dos pinos
  pinMode(BTN_TIME_PIN, INPUT_PULLUP);
  pinMode(BTN_MODE_PIN, INPUT_PULLUP);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(LED_WHITE_PIN, OUTPUT);

  // Inicializa os LEDs
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_BLUE_PIN, LOW);
  digitalWrite(LED_WHITE_PIN, LOW);

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Inicializa OTA (atualização over-the-air)
  ArduinoOTA.setHostname("ServoControlPerfum"); // Nome OTA
  ArduinoOTA.begin();

  // Inicializa o NTP
  timeClient.begin();

  // Inicializa o servo motor
  servo.attach(SERVO_PIN);
  servo.write(0); // Posição inicial do servo
}

void loop() {
  ArduinoOTA.handle(); // Gerencia OTA
  timeClient.update(); // Atualiza a hora via NTP

  handleButtons(); // Verifica os botões

  // Verifica se é hora de acionar o servo
  if (millis() - lastServoActivation >= timeInterval) {
    if (shouldActivateServo()) { // Verifica se deve acionar com base no dia da semana
      activateServo();
    }
  }
}

// Função para acionar o servo
void activateServo() {
  servo.write(0); // Move o servo para 0 graus
  delay(1000); // Aguarda 1 segundo
  servo.write(120); // Retorna para 180 graus

  // Atualiza o último acionamento
  lastServoActivation = millis();

  // Atualiza LEDs
  updateLEDs();
}

// Função para gerenciar os LEDs com base no intervalo de tempo
void updateLEDs() {
  digitalWrite(LED_GREEN_PIN, timeInterval == 45 * 60 * 1000 ? HIGH : LOW);
  digitalWrite(LED_YELLOW_PIN, timeInterval == 30 * 60 * 1000 ? HIGH : LOW);
  digitalWrite(LED_RED_PIN, timeInterval == 15 * 60 * 1000 ? HIGH : LOW);
  digitalWrite(LED_BLUE_PIN, weekendOnly ? HIGH : LOW);
  digitalWrite(LED_WHITE_PIN, weekendOnly ? LOW : HIGH);
}

// Função para verificar os botões
void handleButtons() {
  // Botão para alternar o intervalo de tempo
  if (digitalRead(BTN_TIME_PIN) == LOW && !btnTimePressed) {
    btnTimePressed = true;

    // Alterna o intervalo de tempo
    if (timeInterval == 45 * 60 * 1000) {
      timeInterval = 30 * 60 * 1000; // 30 minutos
    } else if (timeInterval == 30 * 60 * 1000) {
      timeInterval = 15 * 60 * 1000; // 15 minutos
    } else {
      timeInterval = 45 * 60 * 1000; // 45 minutos
    }

    delay(5000); // Aguarda 10 segundos antes de ativar
    activateServo();
  } else if (digitalRead(BTN_TIME_PIN) == HIGH) {
    btnTimePressed = false;
  }

  // Botão para alternar o modo de funcionamento (todos os dias/finais de semana)
  if (digitalRead(BTN_MODE_PIN) == LOW && !btnModePressed) {
    btnModePressed = true;
    weekendOnly = !weekendOnly; // Alterna o modo
    updateLEDs();
  } else if (digitalRead(BTN_MODE_PIN) == HIGH) {
    btnModePressed = false;
  }
}

// Função para verificar se deve acionar com base no dia da semana
bool shouldActivateServo() {
  time_t rawTime = timeClient.getEpochTime();
  struct tm* timeInfo = localtime(&rawTime);

  int dayOfWeek = timeInfo->tm_wday; // 0 = domingo, 6 = sábado

  if (weekendOnly) {
    return (dayOfWeek == 0 || dayOfWeek == 6); // Apenas sábados e domingos
  }
  return true; // Todos os dias
}
