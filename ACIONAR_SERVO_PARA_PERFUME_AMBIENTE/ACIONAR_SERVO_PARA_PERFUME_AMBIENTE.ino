// Bibliotecas
#include <Servo.h>           // Biblioteca para o servo motor
#include <WiFi.h>            // Wi-Fi para ESP32
#include <BlynkSimpleEsp32.h> // Biblioteca do Blynk

// Definições de pinos
#define SERVO_PIN 18       // Pino do servo
#define BUTTON_PIN 10      // Pino do botão para alternar intervalo
#define RELAY_PIN 5        // Pino do relé
#define BLYNK_BUTTON V0    // Botão virtual do Blynk para o relé

// Variáveis globais
Servo servo;
bool tempo_30_min = false;  // Intervalo inicial: 20 minutos (false), 30 minutos (true)
unsigned long ultimoAcionamento = 0;
unsigned long intervalo = 20 * 60 * 1000; // 20 minutos em milissegundos
bool estadoBotaoAnterior = HIGH; // Estado anterior do botão

// Credenciais Wi-Fi e Blynk
char auth[] = "SEU_TOKEN_BLYNK";  // Substitua pelo seu token do Blynk
char ssid[] = "JPL";         // Nome da rede Wi-Fi
char pass[] = "123@juarez";        // Senha do Wi-Fi

void setup() {
  // Inicialização dos pinos
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Botão com pull-up interno
  pinMode(RELAY_PIN, OUTPUT);        // Pino do relé como saída
  digitalWrite(RELAY_PIN, LOW);      // Relé inicialmente desligado

  // Conectando ao Wi-Fi e ao Blynk
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  // Configurando o servo
  servo.attach(SERVO_PIN);
  servo.write(0); // Posiciona o servo no ângulo inicial

  Serial.println("Setup concluído.");
}

void loop() {
  Blynk.run(); // Mantém a conexão com o Blynk

  // Lê o estado do botão e detecta mudanças
  bool estadoBotaoAtual = digitalRead(BUTTON_PIN);
  if (estadoBotaoAtual == LOW && estadoBotaoAnterior == HIGH) {
    // Alterna o intervalo de tempo
    tempo_30_min = !tempo_30_min;
    intervalo = tempo_30_min ? 30 * 60 * 1000 : 20 * 60 * 1000; // 20 ou 30 minutos
    Serial.print("Intervalo alterado para: ");
    Serial.print(intervalo / 60000);
    Serial.println(" minutos.");
  }
  estadoBotaoAnterior = estadoBotaoAtual;

  // Aciona o servo no intervalo configurado
  if (millis() - ultimoAcionamento >= intervalo) {
    acionarServo();
    ultimoAcionamento = millis();
  }
}

// Função para acionar o servo motor
void acionarServo() {
  Serial.println("Servo acionado.");
  servo.write(90); // Gira o servo para 90 graus
  delay(1000);     // Mantém a posição por 1 segundo
  servo.write(0);  // Retorna para a posição inicial
}

// Função chamada pelo botão virtual do Blynk para o relé
BLYNK_WRITE(BLYNK_BUTTON) {
  int estado = param.asInt(); // Lê o valor do botão (0 ou 1)
  digitalWrite(RELAY_PIN, estado); // Liga ou desliga o relé
  Serial.print("Relé ");
  Serial.println(estado ? "ligado" : "desligado");
}
