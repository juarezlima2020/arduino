#define BLYNK_PRINT Serial   
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>
#define BLYNK_PRINT Serial
int sensorData=0;

char auth[] = "RRyBfdhqXAQY5cXT3ndmf7Z5QtK2poKh"; //Authentication code sent by Blynk
char ssid[] = "JPL";                        //WiFi SSID
char pass[] = "123@juarez";                 //WiFi Password

BLYNK_WRITE(V10)
{
  int pinValue = param.asInt();
  digitalWrite(D8, pinValue);
}
void sendSensor()
{

  sensorData = analogRead(A0); //reading the sensor on A0

  if ( isnan(sensorData) ){
    // Serial.println("Failed to read from Hygrometer Soil Moisture sensor!");
    return;
  } else {
    // Serial.println(sensorData);
    // When the plant is watered well the sensor will read a value 380~400, I will keep the 400 
    // value but if you want you can change it below. 
  
    sensorData = constrain(sensorData,400,1023);  //Keep the ranges!
    sensorData = map(sensorData,400,1023,100,0);  //Map value : 400 will be 100 and 1023 will be 0
    // Serial.println(output);

    Blynk.virtualWrite(V1, sensorData);
  }
}
  void setup()
  {
    pinMode(A0, INPUT);
    pinMode(D2, OUTPUT);
    Serial.begin(9600);
    Serial.print(sensorData);
  }
 
