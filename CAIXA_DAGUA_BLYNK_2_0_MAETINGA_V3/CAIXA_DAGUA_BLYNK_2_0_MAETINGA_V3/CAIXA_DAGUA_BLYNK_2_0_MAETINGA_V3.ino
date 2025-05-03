// Adaptado para ESP32 e ESP8266 com correções para OTA
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ArduinoOTA.h>
#include <SimpleTimer.h>

#define BLYNK_TEMPLATE_ID "TMPLF3uOELOF"
#define BLYNK_DEVICE_NAME "Grafico Novo"
#define BLYNK_AUTH_TOKEN "_drkwZR238VCOMjqDbxE0KtrJ4MP2AKi"
#define OTA_HOSTNAME "Maetinga-001-2"

#define TRIGGER 4
#define ECHO 5

// Pinos de controle
int LED = 23;
int cem = 22;
int oitenta = 21;
int sessenta = 19;
int quarenta = 18;
int vinte = 15;

char ssid[] = "Juvenil_MaetingaNET";
char pass[] = "987652017";
char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  // Configuração dos pinos
  pinMode(LED, OUTPUT);
  pinMode(cem, OUTPUT);
  pinMode(oitenta, OUTPUT);
  pinMode(sessenta, OUTPUT);
  pinMode(quarenta, OUTPUT);
  pinMode(vinte, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  // Conectar ao WiFi
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, !digitalRead(LED));
    delay(500);
    Serial.print(".");
  }
  digitalWrite(LED, HIGH);
  Serial.println("\nWi-Fi conectado.");

  // Inicializa Blynk e OTA
  Blynk.begin(auth, ssid, pass);
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.onStart([]() {
    Serial.println("Iniciando OTA...");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA concluído.");
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erro OTA [%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Falha de autenticação");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Erro ao iniciar");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Erro de conexão");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Erro de recebimento");
    else if (error == OTA_END_ERROR) Serial.println("Erro ao finalizar");
  });
  ArduinoOTA.begin();

  Serial.println("OTA pronto.");
}

void loop() {
  long duration, distance;
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) / 29.1;
  Blynk.virtualWrite(V6, distance);

  // Controle de LEDs baseado na distância
  digitalWrite(vinte, distance <= 70 ? HIGH : LOW);
  digitalWrite(quarenta, distance <= 60 ? HIGH : LOW);
  digitalWrite(sessenta, distance <= 50 ? HIGH : LOW);
  digitalWrite(oitenta, distance <= 40 ? HIGH : LOW);
  digitalWrite(cem, distance <= 30 ? HIGH : LOW);

  // Notificações do Blynk
  if (distance <= 30) {
    Blynk.notify("Reservatório está CHEIO!!");
  } else if (distance <= 70) {
    Blynk.notify("Reservatório está VAZIO!!");
  }

  // Atualização OTA e Blynk
  Blynk.run();
  ArduinoOTA.handle();
}
