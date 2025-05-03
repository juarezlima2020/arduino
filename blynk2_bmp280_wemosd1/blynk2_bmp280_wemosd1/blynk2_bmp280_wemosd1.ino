#define BLYNK_TEMPLATE_ID "TMPL2qAHDjEnf"
#define BLYNK_TEMPLATE_NAME "BME 280"
#define BLYNK_AUTH_TOKEN "rhbv2UR36dlVXeTtGYintiDLiekAzmpP"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define APP_DEBUG
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
//#define USE_NODE_MCU_BOARD
#define USE_WEMOS_D1_MINI
#define SEALEVELPRESSURE_HPA (1013.25)
#define BMP280_I2C_ADDRESS  0x76 //define device I2C address: 0x76 or 0x77 (0x77 is library default address)
Adafruit_BMP280 bmp; //For I2C interface //initialize Adafruit BMP280 library
#include "BlynkEdgent.h"
 void setup()
 {
   Serial.begin(9600);
   BlynkEdgent.begin();
   Wire.begin(D2, D1);
   // initialize the BMP280 sensor
   Serial.println("Weather Monitoring Station");
  if( bmp.begin(BMP280_I2C_ADDRESS) == 0 ) 
  {
     // connection error or device address wrong!
     Serial.println("Check wiring of BMP280 sensor");
      }
 }
 void loop()
 {
   BlynkEdgent.run(); // Initiates Blynk
 //read temperature, humidity and pressure from the BMP280 sensor
   float temp = bmp.readTemperature();    // get temperature in degree Celsius
   float pres = bmp.readPressure()/100;   // get pressure in Pa
   float alti = bmp.readAltitude(SEALEVELPRESSURE_HPA); // get altitude in meter
 //print temperature
   Serial.print("Temperature = ");
   Serial.print(temp);
   Serial.println("*C");
 //print pressure
   Serial.print("Pressure = ");
   Serial.print(pres);
   Serial.println("hPa");
 //print Altitude
   Serial.print("Approx. Altitude = ");
   Serial.print(alti);
   Serial.println("m");
   delay(1000);
   Serial.println();
   Blynk.virtualWrite(V0, temp); // For Temperature
   Blynk.virtualWrite(V1, pres); // For Pressure
   Blynk.virtualWrite(V2, alti); //For Approx. Altitude
   delay(1000);
 }
