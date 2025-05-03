//smart water level monitoring system
//https://www.youtube.com/watch?v=zrY_k1Y5y4k&ab_channel=Vinaytechsolutions
//https://www.dropbox.com/sh/c1ja9nbrcc7zkmt/AAAbruV_viRsvAifbinA_3jXa?dl=0&preview=smart+water+level+monitoring+system.txt

//#define BLYNK_TEMPLATE_ID "TMPLMs2io8LM"
//#define BLYNK_DEVICE_NAME "Nível da água"
//#define BLYNK_AUTH_TOKEN "Gniv9gHj4-1_Ts_K-r9WBI-BaQJiCc9j" // pesqueiro.valeverde2022@gmail.com //V@leVerde.2022
//#define OTA_HOSTNAME "HelpCityNivel-001"

#define BLYNK_TEMPLATE_ID "TMPLsyiY8Id-"
#define BLYNK_DEVICE_NAME "NivelCaixaDaguaMaetinga"
#define BLYNK_AUTH_TOKEN "TBk-R1UnanlOizNyAPqogbLL75WyKnFn" //juvenil.lima2040 // Juvenil.1952
#define OTA_HOSTNAME "MAETINGA-CX-DAGUA-001"

#include <ESP8266mDNS.h>  // For OTA with ESP8266
#include <WiFiUdp.h>      // For OTA
#include <ArduinoOTA.h>   // For OTA  
#include <SPI.h>
#include <ESP.h>; 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#define BLYNK_PRINT Serial

#define TRIGGER D0  //pino externo esp01
#define ECHO    D1  //pino interno esp01
int LED       = D4;  
int cem       = D8;
int oitenta   = D7;
int sessenta  = D6;
int quarenta  = D5;
int vinte     = D3;

BlynkTimer timer;

char ssid[] = "JPL |||";
char pass[] = "123@juarez";                //WiFi Password
char auth[] = BLYNK_AUTH_TOKEN;

WiFiClient client;
/*
//implementa wathdog
void WDT_(unsigned long Duration)
{

/*  ESP.wdtDisable();
  unsigned long prevTime=millis();
  while (millis()-prevTime<Duration) 
  {
  ESP.wdtEnable(5000);
  } 
}*/

void setup() 
{
  
// put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Conectando à ");
  Serial.println(ssid);
  pinMode(LED, OUTPUT); // defina o pino digital como saída para o LED de conexão.
//sensor ultra  
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
//leds físicos
  pinMode(cem, OUTPUT);
  pinMode(oitenta, OUTPUT);
  pinMode(sessenta, OUTPUT);
  pinMode(quarenta, OUTPUT);
  pinMode(vinte, OUTPUT);
//inicia wifi  
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
  delay(1000);             // espere 1 segundo.
  
  Blynk.begin(auth, ssid, pass);
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // Para OTA - Use seu próprio nome de identificação do dispositivo
  ArduinoOTA.begin();                    // For OTA

  
//setar tempo pra reiniciar
//  WDT_(10000);

  Blynk.config(auth);
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
  Blynk.virtualWrite(V6, distance);
  Serial.print(distance);
  Serial.println("Cm antes de converter:");
  delay(1000);
  //distance = map(distance,0,100,100,0);

   if (distance <=100) 
   {
    Blynk.virtualWrite(V0, 255); // 
    digitalWrite(vinte, HIGH);
    Blynk.notify("Reservatório está VAZIO!!");
   }
  else 
  {
    Blynk.virtualWrite(V0, 0);
    digitalWrite(vinte, LOW);
  }

 if (distance <= 80) 
 {
    Blynk.virtualWrite(V1, 255); // 40 cm
    digitalWrite(quarenta, HIGH);
    //Blynk.notify("Atenção, reservatório abaixo de 40%!!");
 }
  else 
  {
    Blynk.virtualWrite(V1, 0);
    digitalWrite(quarenta, LOW);
  }

   if (distance <= 60) 
   {
    Blynk.virtualWrite(V2, 255); // 60 cm
    digitalWrite(sessenta, HIGH);
   }
  else 
  {
    Blynk.virtualWrite(V2, 0);
    digitalWrite(sessenta, LOW);
  }

   if (distance <= 40) 
   {
    Blynk.virtualWrite(V3, 255); // 80cm
    digitalWrite(oitenta, HIGH);
   }
  else 
  {
    Blynk.virtualWrite(V3, 0);
    digitalWrite(oitenta, LOW);
  }

   if (distance <= 20) 
   {
    Blynk.virtualWrite(V4, 255); // 100 cm
    digitalWrite(cem, HIGH);
    Blynk.notify("Reservatório está CHEIO!!");
   }
  else 
  {
    Blynk.virtualWrite(V4, 0);
    digitalWrite(cem, LOW);
  }
  
  //Serial.println("Centimeter:");
  distance = map(distance,0,100,100,00);
  Serial.print(distance);
  Serial.println("Cm depois de converter:");
  Blynk.virtualWrite(V5, distance); // V5 = Distância
  delay(200);
   Blynk.virtualWrite(V10, 0);      // V10 = ligado
   delay(200);
   Blynk.virtualWrite(V10, 255);
   //Blynk.notify("Reservatório está VAZIO!!");
  delay(100);
  Blynk.run();
  ArduinoOTA.handle();              // For OTA 

}
