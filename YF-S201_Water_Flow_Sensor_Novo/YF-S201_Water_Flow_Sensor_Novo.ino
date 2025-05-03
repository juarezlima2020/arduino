//https://www.electronicshub.org/arduino-water-flow-sensor-interface/

#define BLYNK_TEMPLATE_ID "TMPL6aeTY_DN"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "bKM2zM9TSl4ETrENo2II3qupcWdnpqM6"// juarez.lima2030@gmail.com
#define OTA_HOSTNAME "FluxoAguaBlynk-001"

#include <ESP8266mDNS.h>  // For OTA with ESP8266
#include <WiFiUdp.h>      // For OTA
#include <ArduinoOTA.h>   // For OTA  
#include <SPI.h>
#include <ESP.h>; 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#define BLYNK_PRINT Serial


const int watermeterPin = D2;
int LED  = D4;  //= D4;
volatile int  pulse_frequency;
unsigned int  literperhour;
unsigned long currentTime, loopTime;
byte sensorInterrupt = D0;

BlynkTimer timer;

char ssid[] = "JPL";                        //WiFi SSID
char pass[] = "123@juarez";                 //WiFi Password
char auth[] = BLYNK_AUTH_TOKEN;

WiFiClient client;

void setup()
{ 
       // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Conectando à ");
  Serial.println(ssid);
  pinMode(LED, OUTPUT); // defina o pino digital como saída para o LED de conexão.
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED, LOW);
    delay(150);
    Serial.print(".");
    digitalWrite(LED, HIGH);
    delay(150);
  }
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Wi-Fi conectado.");
  
  digitalWrite(LED, HIGH); // ligue o led.
  delay(200);         // espere 1 segundo.
  
  Blynk.begin(auth, ssid, pass);
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // Para OTA - Use seu próprio nome de identificação do dispositivo
  ArduinoOTA.begin();  // For OTA
  
   pinMode(watermeterPin, INPUT);
   Serial.begin(9600); 
   attachInterrupt(sensorInterrupt, getFlow, FALLING);
                                     
   currentTime = millis();
   loopTime = currentTime;
 
    Blynk.config(auth);
} 

void getFlow ()
{ 
   pulse_frequency++;
} 

void loop ()    
{
   currentTime = millis();
   if(currentTime >= (loopTime + 1000))
   {
      loopTime = currentTime;
      literperhour = (pulse_frequency * 60 / 7.5);
      pulse_frequency = 0;
      Serial.print(literperhour, DEC);
      Serial.println(" Liter/hour");
   }

     Blynk.run();
     ArduinoOTA.handle();  // For OTA 
}
