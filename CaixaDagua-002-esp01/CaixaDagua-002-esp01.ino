// You should get Auth Token in the Blynk App.
#define BLYNK_TEMPLATE_ID "TMPLpYfPmpB1"
#define BLYNK_DEVICE_NAME "Caixa Dagua 002 esp01"
#define BLYNK_AUTH_TOKEN "O1ba_zLitQfz2RiA-ankHEx6hpRp7LPv" //by juarez.lima2020@gmail.com

//-------- Bibliotecas -----------
#define BLYNK_PRINT Serial   
#include <ESP8266mDNS.h>  // For OTA with ESP8266
#include <WiFiUdp.h>      // For OTA
#include <ArduinoOTA.h>   // For OTA  
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#define BLYNK_PRINT Serial

char ssid[] = "JPL";                        //WiFi SSID
char pass[] = "123@juarez";                 //WiFi Password
char auth[] = BLYNK_AUTH_TOKEN;

#define TRIGGER 2 //0
#define ECHO    0 //2
#define OTA_HOSTNAME "CaixaDagua-002"

WiFiClient client;
   
SimpleTimer timer;

void setup() 
{
  // put your setup code here, to run once:

  Serial.println("Conectando à ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
Blynk.virtualWrite(V10, 255);
delay(200);
Blynk.virtualWrite(V10, 0);
delay(200);
  }
  
  Serial.println("");
  Serial.println("Wi-Fi conectado.");
  
  
  
   Serial.begin(9600);
   Blynk.begin(auth, ssid, pass);
   ArduinoOTA.setHostname(OTA_HOSTNAME);  // Para OTA - Use seu próprio nome de identificação do dispositivo
   ArduinoOTA.begin();  // For OTA
  
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  
  Blynk.config(auth);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  Blynk.run();
  ArduinoOTA.handle();  // For OTA  
  timer.run(); 
  
 long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(50); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(50); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;

   if (distance <=100) {
    Blynk.virtualWrite(V0, 255);
    Blynk.virtualWrite(V5, distance);
    
 
}
  else {
    Blynk.virtualWrite(V0, 0);
   
    
  }

 if (distance <= 80) {
    Blynk.virtualWrite(V1, 255);
    
}
  else {
    Blynk.virtualWrite(V1, 0);
   
  }

   if (distance <= 60) {
    Blynk.virtualWrite(V2, 255);
    
}
  else {
    Blynk.virtualWrite(V2, 0);
    
  }

   if (distance <= 40) {
    Blynk.virtualWrite(V3, 255);
    
}
  else {
    Blynk.virtualWrite(V3, 0);
   
  }

   if (distance <= 20) {
    Blynk.virtualWrite(V4, 255);
   
}
  else {
    Blynk.virtualWrite(V4, 0);
   
    
  }
  Serial.begin(9600);
  Serial.print(distance);
  Serial.println("Centimeter:");
  Blynk.virtualWrite(V5, distance);
  
  delay(500);
  
}
