#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

// Defina suas credenciais Wi-Fi
const char* ssid = "JPL";
const char* password = "123@juarez";

// Nome do dispositivo OTA
const char* deviceName = "AlcoolGelDevice002";

#define TRIGGER_PIN D1
#define ECHO_PIN    D2
#define SERVO_PIN   D5
#define LED1_PIN    D6
#define LED2_PIN    D7
#define LED3_PIN    D8

Servo servo;
bool wifiConnected = false;

// Função para medir a distância ultrassônica
long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

// Função para conectar ao Wi-Fi
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Conectando ao Wi-Fi...");
  
  int attempts = 0; // Limite de tentativas para evitar loop infinito

  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    digitalWrite(LED_BUILTIN, LOW); // Liga o LED incorporado (pisca)
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH); // Desliga o LED incorporado
    delay(250);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    digitalWrite(LED_BUILTIN, LOW); // Mantém o LED ligado
    Serial.println("\nWi-Fi conectado!");
    Serial.printf("Endereço IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    wifiConnected = false;
    digitalWrite(LED_BUILTIN, HIGH); // Deixa o LED desligado
    Serial.println("\nFalha ao conectar ao Wi-Fi. Continuando sem conexão...");
  }
}

void setup() {
  // Inicializa o Serial Monitor
  Serial.begin(115200);

  // Inicializa o servo
  servo.attach(SERVO_PIN);

  // Configura os pinos de saída
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH); // LED desligado inicialmente

  // Conecta ao Wi-Fi
  connectToWiFi();

  // Configura OTA apenas se estiver conectado ao Wi-Fi
  if (wifiConnected) {
    ArduinoOTA.setHostname(deviceName);

    ArduinoOTA.onStart([]() {
      String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
      Serial.println("Iniciando OTA: " + type);
    });

    ArduinoOTA.onEnd([]() {
      Serial.println("\nOTA finalizada.");
    });

    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Erro[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Erro de autenticação");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Erro no início");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Erro de conexão");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Erro de recebimento");
      else if (error == OTA_END_ERROR) Serial.println("Erro no final");
    });

    ArduinoOTA.begin();
    Serial.printf("OTA configurado. Nome do dispositivo: %s\n", deviceName);
  }
}

void loop() {
  // Verifica e processa atualizações OTA apenas se conectado ao Wi-Fi
  if (wifiConnected) {
    ArduinoOTA.handle();
  }

  // Controle do servo e LEDs com base na distância
  float distance = 0.01723 * readUltrasonicDistance(TRIGGER_PIN, ECHO_PIN);
  Serial.printf("Distância medida: %.2f cm\n", distance);

  if (distance <= 10) {
    servo.write(0);
    digitalWrite(LED1_PIN, HIGH);
    delay(1000); // Aguarda 1 segundo
    servo.write(75);
    digitalWrite(LED1_PIN, LOW);
    delay(1000); // Aguarda 1 segundo
  } else if (distance > 11 && distance < 100) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(LED3_PIN, LOW);
      delay(100); // Aguarda 100 ms
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, HIGH);
      delay(100); // Aguarda 100 ms
    }
  } else {
    // Se nenhuma condição for atendida, desliga os LEDs
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
  }
}
