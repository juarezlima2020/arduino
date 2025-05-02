#define BLYNK_TEMPLATE_ID "TMPLTEiwCxHa"
#define BLYNK_DEVICE_NAME "Contrasseco"
#define BLYNK_AUTH_TOKEN "Pc1hoElXrKLFMsOPfMduB41pC5ZHXT2i" //2014jplima

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

BlynkTimer timer;

void checkPhysicalButton();

int relay1State = LOW;
int pushButton1State = HIGH;

int relay2State = LOW;
int pushButton2State = HIGH;

int relay3State = LOW;
int pushButton3State = HIGH;

//int relay4State = LOW;
//int pushButton4State = HIGH;

#define RELAY_PIN_1      16   //D0
#define RELAY_PIN_2       5   //D1
#define RELAY_PIN_3       4   //D2
#define ledvermelho       0   //D3 

#define PUSH_BUTTON_1    14   //D5 
#define PUSH_BUTTON_2    12   //D6
#define PUSH_BUTTON_3    13   //D7
#define boia     1   //TX OK

#define VPIN_BUTTON_1    V12 
#define VPIN_BUTTON_2    V13
#define VPIN_BUTTON_3    V14
#define VPIN_BUTTON_4    V15  

#define OTA_HOSTNAME "HelpCity"

BLYNK_CONNECTED() {

  // Solicitar o estado mais recente do servidor

   Blynk.syncVirtual(VPIN_BUTTON_1);
   Blynk.syncVirtual(VPIN_BUTTON_2);
   Blynk.syncVirtual(VPIN_BUTTON_3);
   Blynk.syncVirtual(VPIN_BUTTON_4);

}

// Quando o botão App é pressionado - mude o estado

BLYNK_WRITE(VPIN_BUTTON_1) 
{
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}

BLYNK_WRITE(VPIN_BUTTON_2) 
{
  relay2State = param.asInt();
  digitalWrite(RELAY_PIN_2, relay2State);
}
BLYNK_WRITE(VPIN_BUTTON_3) 
{
  relay3State = param.asInt();
  digitalWrite(RELAY_PIN_3, relay3State);
}
BLYNK_WRITE(boia) 
{
  boia = param.asInt();
  digitalWrite(ledvermelho, boia);
}

void checkPhysicalButton()
{
  if (digitalRead(PUSH_BUTTON_1) == LOW) 
  {
    // pushButton1State é usado para evitar alternâncias sequenciais
    if (pushButton1State != LOW) 
    {
      // Alternar estado do relé
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);

      // Botão Atualizar Widget
      Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
    }
    pushButton1State = LOW;
  } 
    else 
  {
    pushButton1State = HIGH;
  }

  if (digitalRead(PUSH_BUTTON_2) == LOW) 
  {
    // pushButton2State é usado para evitar alternâncias sequenciais
    if (pushButton2State != LOW) 
    {
      // Alternar estado do relé
      relay2State = !relay2State;
      digitalWrite(RELAY_PIN_2, relay2State);

      // Botão Atualizar Widget
      Blynk.virtualWrite(VPIN_BUTTON_2, relay2State);
    }
    pushButton2State = LOW;
  } 
    else 
  {
    pushButton2State = HIGH;
  }

  if (digitalRead(PUSH_BUTTON_3) == LOW) 
  {
    // pushButton3State é usado para evitar alternâncias sequenciais
    if (pushButton3State != LOW) 
    {
      // Alternar estado do relé
      relay3State = !relay3State;
      digitalWrite(RELAY_PIN_3, relay3State);

      // Botão Atualizar Widget
      Blynk.virtualWrite(VPIN_BUTTON_3, relay3State);
    }
    pushButton3State = LOW;
  } 
    else 
  {
    pushButton3State = HIGH;
  }

  if (digitalRead(boia) == LOW && PUSH_BUTTON_3 == HIGH) 
  {
    // pushButton4State is used to avoid sequential toggles
    if (pushButton4State != LOW && pushButton3State == HIGH) 
    {
      // Alternar estado do relé
      boia = HIGH;
      digitalWrite(ledvermelho, HIGH);

      // Botão Atualizar Widget
      Blynk.virtualWrite(VPIN_BUTTON_4, HIGH);
    }
    pushButton4State = LOW;
  } 
    else 
  {
    pushButton4State = HIGH;
  }
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // Para OTA - Use seu próprio nome de identificação do dispositivo
  ArduinoOTA.begin();  // For OTA

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, relay1State);

  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(PUSH_BUTTON_2, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_2, relay2State);

  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(PUSH_BUTTON_3, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_3, relay3State);

  pinMode(ledvermelho, OUTPUT);
  pinMode(boia, INPUT_PULLUP);
  digitalWrite(ledvermelho, HIGH);

  Blynk.config(auth);
  // Configure uma função a ser chamada a cada 200 ms
  timer.setInterval(200L, checkPhysicalButton);
}

void loop()
{
  Blynk.run();
  ArduinoOTA.handle();  // For OTA
  timer.run();
}
