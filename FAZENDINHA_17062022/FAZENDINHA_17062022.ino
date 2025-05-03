/*************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App project setup:
    Value Display widget attached to V5
    Value Display widget attached to V6
 *************************************************************/

// Template ID, Device Name e Auth Token são fornecidos pelo Blynk.Cloud
// Veja a guia Device Info ou as configurações do modelo
#define BLYNK_TEMPLATE_ID "TMPLrKKyt9wX"
#define BLYNK_DEVICE_NAME "HortaCasaErika"
#define BLYNK_AUTH_TOKEN "6qQSYp2iIGHYeyuy_6KFLBpMlkVlINI9" //Authentication code sent by Blynk

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

int sensorPinD = D3;
int sensorPin = A0;    // selecione o pino de entrada para o potenciômetro
int enable1 = D5;      // habilitar a leitura do sensor A
int enable2 = D6;      // habilitar a leitura do sensor b
int enable3 = D7;      // habilitar a leitura do sensor c

float sensorValue1 = 0.000;  // variável para armazenar o valor vindo do sensor A
float sensorValue2 = 0.00;  // variável para armazenar o valor vindo do sensor B
float sensorValue3 = 0.0;  // variável para armazenar o valor vindo do sensor C
char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "JPL";
char pass[] = "123@juarez";


#define DHTPIN D2          // What digital pin we're connected to for T and H
#define LED D4           // Led in NodeMCU at pin GPIO16 (D0) for connection LED.

// DHT Type
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
WiFiClient client;


// Send it to Blynk
void sendSensor()
{
  // read the value from sensor A:
 // digitalWrite(enable1, HIGH); 
  Serial.println("Reading Sensor 1");
  delay(5000);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {

    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, t);
  Serial.print(t);
  Serial.println(" C");
  Blynk.virtualWrite(V1, h);
  Serial.print(h);
  Serial.println(" %");
  delay(1000);
 // digitalWrite(enable1, LOW);
  
}

void setup()
{
  // Debug console
  Serial.begin(9600); // default was 9600
  delay(10);
 // Blynk.begin(auth, ssid, pass);
 Blynk.config(auth);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  
  dht.begin();
  
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);

  //pinMode(enable1, OUTPUT); //Set D5 to OutPut to Switch on Sensor1
  pinMode(enable2, OUTPUT); //Set D6 to OutPut to Switch on Sensor2
  pinMode(enable3, OUTPUT); //Set D7 to OutPut to Switch on Sensor3
  
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

}
void sensordata()
{

 // read the value from sensor B:
  digitalWrite(enable2, HIGH);
  Serial.println("Reading Sensor 2");
  delay(5000); 
  //sensorValue2 = (100 - ( (analogRead(sensorPin)/1023) * 100 ));
  //sensorValue2 = (((analogRead(sensorPin)/353) * 100 ));
  sensorValue2 = analogRead(sensorPin);
  sensorValue2 = map(sensorValue2,0,1023,100,0);
  Blynk.virtualWrite(V2, sensorValue2);
  Serial.println(sensorValue2);
  delay(1000);
  digitalWrite(enable2, LOW);
  

  // read the value from sensor C:
  digitalWrite(enable3, HIGH); 
  Serial.println("Reading Sensor 3");
  delay(5000);
  //sensorValue3 = (100 - ( (analogRead(sensorPin)/1023) * 100 ));
  //sensorValue3 = (((analogRead(sensorPin)/353) * 100 ));
  sensorValue3 = digitalRead(sensorPinD);
  sensorValue3 = map(sensorValue3,0,1,1,0);
  Blynk.virtualWrite(V3, sensorValue3);
  Serial.println(sensorValue3);
  delay(1000);
  digitalWrite(enable3, LOW);
  Serial.println("----------------------------------------");
 // delay before starting next set of readings
 // ESP.deepSleep(1 * 300000000);//Dorme por 5 Minutos (Deep-Sleep em Micro segundos).
}


void loop()
{
  Blynk.run(); 
  timer.run(); 
  sensordata();
}
