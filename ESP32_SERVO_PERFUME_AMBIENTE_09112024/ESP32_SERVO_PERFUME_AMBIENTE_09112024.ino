#include <ESP8266WiFi.h>    // Para ESP8266, use a biblioteca específica. Para ESP32-C3, troque para ESP32.
#include <BlynkSimpleEsp8266.h>  // Para ESP8266, use esta biblioteca. Para ESP32-C3, use BlynkSimpleEsp32.h.
#include <Servo.h>
#include <ArduinoOTA.h>

// Definições de pinos
#define PINO_SERVO      D1       // Pino do servo motor (ajuste conforme seu hardware)
#define PINO_BOTAO      D2       // Pino do botão físico para alternar os tempos
#define PINO_LED_VERMELHO D5    // LED vermelho (intervalo de 15 minutos)
//#define PINO_LED_AMARELO D6;    // LED amarelo (intervalo de 30 minutos)
#define PINO_LED_VERDE   D7    // LED verde (intervalo de 45 minutos)
#define PINO_LED_BRANCO  D8    // LED branco (para confirmar a troca de tempo)

// Definindo o Servo
Servo servo;
int angulo = 0;  // Ângulo do servo (0 ou 40 graus)

// Variáveis para controle
int intervalo = 15;       // Intervalo de tempo (15, 30 ou 45 minutos)
unsigned long ultimaExecucao = 0;
int contador = 0;

// Definindo o horário de funcionamento via Blynk
bool diasAtivos[7] = {true, true, true, true, true, true, true};  // Todos os dias ativos por padrão

// Definições do Blynk
char auth[] = "SEU_AUTH_TOKEN";
char ssid[] = "SUA_REDE_WIFI";
char pass[] = "SUA_SENHA_WIFI";

// Variáveis de controle de tempo
unsigned long tempoAtual = 0;
unsigned long tempoAnterior = 0;

// Função para alterar o intervalo de tempo
void alterarIntervalo() {
  if (intervalo == 15) {
    intervalo = 30;
    digitalWrite(PINO_LED_AMARELO, HIGH);
    digitalWrite(PINO_LED_VERMELHO, LOW);
  } else if (intervalo == 30) {
    intervalo = 45;
    digitalWrite(PINO_LED_VERDE, HIGH);
    digitalWrite(PINO_LED_AMARELO, LOW);
  } else if (intervalo == 45) {
    intervalo = 15;
    digitalWrite(PINO_LED_VERMELHO, HIGH);
    digitalWrite(PINO_LED_VERDE, LOW);
  }
  piscarLED(PINO_LED_BRANCO, 3);  // Pisca o LED branco confirmando a troca
  Blynk.virtualWrite(V1, intervalo);  // Atualiza o intervalo no app Blynk
}

// Função para piscar o LED de confirmação
void piscarLED(int pino, int vezes) {
  for (int i = 0; i < vezes; i++) {
    digitalWrite(pino, HIGH);
    delay(500);
    digitalWrite(pino, LOW);
    delay(500);
  }
}

// Função de controle do servo motor
void acionarServo() {
  // Verifica o dia da semana e só aciona se o dia estiver ativo
  int diaDaSemana = weekday() - 1;  // Ajuste para domingo=0 até sábado=6
  if (diasAtivos[diaDaSemana]) {
    // Aciona o servo para 40 graus por 1 segundo
    servo.write(40);  
    delay(1000);
    servo.write(0);   // Retorna para 0 graus
    contador++;  // Incrementa o contador
    Blynk.virtualWrite(V2, contador);  // Atualiza o contador no app Blynk
  }
}

// Função para atualizar a configuração OTA
void configuraOTA() {
  ArduinoOTA.onStart([]() {
    String tipo = ArduinoOTA.getCommand() == U_FLASH ? "sketch" : "filesystem"; // Verifica tipo de atualização
    String nome = ArduinoOTA.getSketchName();
    Serial.println("Iniciando atualização " + tipo + ": " + nome);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nFim da atualização");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Falha de autenticação");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Falha ao iniciar");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Falha de conexão");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Falha de recebimento");
    } else if (error == OTA_END_ERROR) {
      Serial.println("Falha de fim");
    }
  });
  ArduinoOTA.begin();
}

void setup() {
  Serial.begin(115200);
  
  // Configuração dos pinos
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_LED_VERMELHO, OUTPUT);
  pinMode(PINO_LED_AMARELO, OUTPUT);
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_BRANCO, OUTPUT);
  
  servo.attach(PINO_SERVO);  // Conecta o servo no pino especificado
  
  Blynk.begin(auth, ssid, pass);
  configuraOTA();  // Configura a atualização OTA
  
  // Inicializa LEDs com intervalos padrões
  digitalWrite(PINO_LED_VERMELHO, HIGH);
  digitalWrite(PINO_LED_AMARELO, LOW);
  digitalWrite(PINO_LED_VERDE, LOW);
  
  // Chama a função para alterar o intervalo ao pressionar o botão
  pinMode(PINO_BOTAO, INPUT_PULLUP);
}

void loop() {
  Blynk.run();
  ArduinoOTA.handle();

  tempoAtual = millis();
  if (digitalRead(PINO_BOTAO) == LOW) {
    alterarIntervalo();
    delay(300);  // Debounce do botão
  }

  // Verifica se o intervalo de tempo foi atingido
  if (tempoAtual - tempoAnterior >= intervalo * 60000) {
    acionarServo();
    tempoAnterior = tempoAtual;  // Reseta o tempo anterior
  }
}
