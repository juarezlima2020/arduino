#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Adafruit_NeoPixel.h>

// Configurações da rede WiFi
//const char* ssid = "JPL";
//const char* password = "123@juarez";

char ssid[] = "JPL";                        //WiFi SSID
char pass[] = "123@juarez"; 

// Configurações do NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0; // Fuso horário (em segundos) do UTC (sem ajuste de horário de verão)
const int daylightOffset_sec = 0; // Ajuste de horário de verão (em segundos)

// Configurações dos LEDs
#define LED_PIN 5        // Pino de controle dos LEDs
#define NUM_LEDS 7       // Número de LEDs
#define BRIGHTNESS 100   // Brilho dos LEDs (0-255)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

void setup() {
  Serial.begin(115200);
  
  // Inicializa a conexão WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Inicializa o cliente NTP
  timeClient.begin();
  timeClient.update(); // Atualiza para obter a hora atual
  
  // Inicializa os LEDs
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Mostra a cor inicial (preto)

  Serial.println("Setup complete");
}

void loop() {
  timeClient.update(); // Atualiza o tempo do servidor NTP

  int dayOfWeek = timeClient.getDay();
  Serial.print("Day of week: ");
  Serial.println(dayOfWeek);

  // Define as cores dos LEDs com base no dia da semana
  switch (dayOfWeek) {
    case 1: // Domingo (vermelho)
      setLEDColor(strip.Color(255, 0, 0));
      break;
    case 2: // Segunda-feira (laranja)
      setLEDColor(strip.Color(255, 165, 0));
      break;
    case 3: // Terça-feira (amarelo)
      setLEDColor(strip.Color(255, 255, 0));
      break;
    case 4: // Quarta-feira (verde)
      setLEDColor(strip.Color(0, 255, 0));
      break;
    case 5: // Quinta-feira (azul)
      setLEDColor(strip.Color(0, 0, 255));
      break;
    case 6: // Sexta-feira (índigo)
      setLEDColor(strip.Color(75, 0, 130));
      break;
    case 7: // Sábado (violeta)
      setLEDColor(strip.Color(238, 130, 238));
      break;
    default:
      // Caso não haja correspondência, desliga todos os LEDs
      setLEDColor(strip.Color(0, 0, 0));
      break;
  }

  delay(60000); // Aguarda um minuto antes de atualizar novamente
}

void setLEDColor(uint32_t color) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
