/*
#define BLYNK_TEMPLATE_ID "TMPLWn1betQW"
#define BLYNK_DEVICE_NAME "HortencioCasa"
#define BLYNK_AUTH_TOKEN "ZCVvghIP4JIDnrC1wJyTdoTn0ZRDhG6J"
*/

#define BLYNK_TEMPLATE_ID "TMPLP2td4mLH"
#define BLYNK_DEVICE_NAME "Hortencio"
#define BLYNK_AUTH_TOKEN "JSSZgenCoQwyJ41LF8SuFEaq7uItEBoD"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
//#include <ESP.h>

int sensorPin = A0;
int sensorPinR = D3;// selecione o pino de entrada para o potenciômetro
int enable1 = D5;      // habilitar a leitura do sensor A
int enable2 = D8;      // habilitar a leitura do sensor b
int enableDHT11 = D7;      // habilitar a leitura do sensor c

float sensorValue1 = 0.00;  // variável para armazenar o valor vindo do sensor A
float sensorValue2 = 0.0;  // variável para armazenar o valor vindo do sensor B
//float sensorValue3 = 0.0;  // variável para armazenar o valor vindo do sensor C

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "JPL";
char pass[] = "123@juarez";

#define DHTPIN D2          // What digital pin we're connected to
#define LED D1            // Led in NodeMCU at pin GPIO16 (D0) for connection LED.
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
WiFiClient client;

void sendSensor()
{
  digitalWrite(enableDHT11, HIGH); 
  Serial.println("---Ativando DHT11 em D7---");
  delay(3000);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.println("Lendo DHT11");
  Blynk.virtualWrite(V1, h);
  Serial.print(h);
  Serial.println(" %");
  Blynk.virtualWrite(V0, t);
  Serial.print(t);
  Serial.println(" C");
  digitalWrite(enableDHT11, LOW);
  Serial.println("-----Desativando DHT11 em D7-----");
  delay(3000);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);

  pinMode(enable1, OUTPUT); //Set D5 to OutPut to Switch on Sensor1
  pinMode(enable2, OUTPUT); //Set D6 to OutPut to Switch on Sensor2
  pinMode(enableDHT11, OUTPUT); //Set D7 to OutPut to Switch on Sensor3
  pinMode(LED, OUTPUT); // set the digital pin D1 as output for connection LED.

  Serial.println("Conectando em:  ");
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
{// read the value from sensor A:
  digitalWrite(enable1, HIGH);
  Serial.println("Ativando Sensor Solo em D5"); 
  Serial.println("Lendo Sensor de solo");
  delay(2000);
  //sensorValue1 = (((analogRead(sensorPin)/1023) * 100 ));
  sensorValue1 = analogRead(sensorPin);
  sensorValue1 = map(sensorValue1,280,700,100,0);
  Blynk.virtualWrite(V2, sensorValue1);
  Serial.print("Umidade do solo = ");
  Serial.print(sensorValue1);
  Serial.println("% ");
  Serial.println("---Destivando Sensor Solo em D5--");
  digitalWrite(enable1, LOW);
  delay(2000);

 // read the value from sensor B:
  Serial.println("Ativando Sensor Chuva em D8");
  digitalWrite(enable2, HIGH);
  Serial.println("Lendo sensor de chuva ");
  delay(2000); 
  //sensorValue2 = (100 - ( (analogRead(sensorPin)/1023) * 100 ));
  //sensorValue2 = (((analogRead(sensorPin)/353) * 100 ));
  sensorValue2 = digitalRead(sensorPinR);
  sensorValue2 = map(sensorValue2,0,1,1,0);
  Blynk.virtualWrite(V3, sensorValue2);
  Serial.println(sensorValue2);
  Serial.println("Desativando Sensor Chuva em D8");
  digitalWrite(enable2, LOW);
  delay(2000);
/*
  // read the value from sensor C:
  digitalWrite(enable3, HIGH); 
  Serial.println("Reading Sensor 3");
  delay(15000);
  //sensorValue3 = (100 - ( (analogRead(sensorPin)/1023) * 100 ));
  //sensorValue3 = (((analogRead(sensorPin)/353) * 100 ));
  sensorValue3 = analogRead(sensorPin);
  Blynk.virtualWrite(V9, sensorValue3);
  Serial.println(sensorValue3);
  digitalWrite(enable3, LOW);
  Serial.println("----------------------------------------");
  delay(10000); // delay before starting next set of readings
  */
  Serial.println("----------------1 minuto-----------------");
 //ESP.deepSleep(1 * 300000000);//Dorme por 5 Minutos (Deep-Sleep em Micro segundos).
// ESP.deepSleep(1 * 60000000);//Dorme por 1 Minuto (Deep-Sleep em Micro segundos).
  
}


void loop()
{
  Blynk.run();
  timer.run();
  sensordata();
}
