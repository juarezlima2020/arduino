//ESP32 XIAO
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

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup() 
{
  pinMode(LED, OUTPUT); // defina o pino digital como saída para o LED de conexão.
  pinMode(LED2, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
 // pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(9600);
 
}

void loop()
{ 
  if (0.01723 * readUltrasonicDistance(D1, D2) > 20 && 0.01723 * readUltrasonicDistance(D1, D2) < 30) {
    digitalWrite(LED, HIGH);
    digitalWrite(LED2, HIGH);
    delay(150);
    Serial.println(" OBSTÁCULO ");
    
  } else {
    digitalWrite(LED, LOW);
    digitalWrite(LED2, LOW);
    delay(150);
    Serial.println(" SEM OBSTÁCULO ");
    
  }
  delay(10); // Delay a little bit to improve simulation performance
}
