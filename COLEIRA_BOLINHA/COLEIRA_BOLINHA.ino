
//#include <ESP8266mDNS.h>  // For OTA with ESP8266
//#include <WiFiUdp.h>      // For OTA
//#include <ArduinoOTA.h>   // For OTA  
#include <SPI.h>
#include <ESP.h>; 
//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#define BLYNK_PRINT Serial

#define TRIGGER D1 
#define ECHO    D2 
int LED       = D4; 
int LED2      = D5;
int LED3      = D8;

void setup() 
{
  pinMode(LED, OUTPUT); // defina o pino digital como saída para o LED de conexão.
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(9600);
 
}

void loop() 
{
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(20); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(100); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;

  Serial.print(distance);
  Serial.println("__Cm antes de converter:");
  delay(200);
  //distance = map(distance,0,100,100,0);

   if (distance <=30) 
   {
    digitalWrite(LED, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    delay(150);
    Serial.print(" < 45 HIGH ");
   }
  else 
  {        
    digitalWrite(LED, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    delay(150);
    Serial.print(" < 45...LOW");
   }
   
  Serial.print(distance);
  Serial.println("Centimeter:");
  distance = map(distance,0,100,100,00);
}
