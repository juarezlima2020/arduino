// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLWn1betQW"
#define BLYNK_DEVICE_NAME "HortencioCasa"
#define BLYNK_AUTH_TOKEN "ZCVvghIP4JIDnrC1wJyTdoTn0ZRDhG6J"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
int solo1 = A0;      // habilitar a leitura do sensor A
int sensorChuva = D3;    // selecione o pino de entrada para o potenciômetro
float sensorSolo1 = 0.000;  // variável para armazenar o valor vindo do sensor A

char ssid[] = "JPL";
char pass[] = "123@juarez";

#define DHTPIN D2          // What digital pin we're connected to
#define LED D4
const int GPIO_0 = D1; //PINO DIGITAL UTILIZADO PELO RELÉ DO MÓDULO 

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
WiFiClient client;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V1, h);
  Serial.print(h);
  Serial.println(" %");
  
  Blynk.virtualWrite(V0, t);
  Serial.print(t);
  Serial.println(" C");
}

void setup()
{

   pinMode(GPIO_0, OUTPUT); //DEFINE O PINO COMO SAÍDA
   digitalWrite(GPIO_0, HIGH); //RELÉ INICIA DESLIGADO
  
   Serial.begin(115200);
   pinMode(LED, OUTPUT); // set the digital pin as output for connection LED.
 
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED, LOW);
    delay(150);
    Serial.print(".");
    digitalWrite(LED, HIGH);
    delay(150);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  digitalWrite(LED, HIGH); // turn the LED on.
  delay(1000);         // wait for 1 second.

  // Debug console
  
  Blynk.begin(auth, ssid, pass);
 
  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void sensordata()
{// Leitura do sensor de solo 1:
  
  Serial.println("Lendo Sensor de Solo 1");
  sensorSolo1 = analogRead(sensorSolo1);
  Blynk.virtualWrite(V2, sensorSolo1);
  Serial.println(sensorSolo1);
  delay(1000);

 
}


void loop()
{
  Blynk.run();
  timer.run();
}
