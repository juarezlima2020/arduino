#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SFE_BMP180.h>
#include <Wire.h>
 
#include "index.h" //Our HTML dashboard page
#include "DHTesp.h" //DHT11 Library for ESP
 
 ///Connect SCL to D1 and Connect SDA to D2
#define LED 2 //On board LED
#define DHTpin 12 // Digital pin 6 (GPIO 12) connected to the DHT sensor
#define rainDigital D0
//#define DHTpin 5 //D1 of NodeMCU is GPIO5
 
SFE_BMP180 pressure;
 
#define ALTITUDE 542.0 // Your Altitude in meters search in google
 
DHTesp dht;
 
//SSID and Password of your WiFi router
const char* ssid = "AndroidAP3DEC";
const char* password = "electroniclinic";
 
ESP8266WebServer server(80); //Server on port 80
 
void handleRoot() {
String s = MAIN_page; //Read HTML contents
server.send(200, "text/html", s); //Send web page
}
 
float humidity, temperature;
 
void handleADC() {
char status;
double T,P,p0,a;
double Tdeg, Tfar, phg, pmb;
 
status = pressure.startTemperature();
if (status != 0)
{
// Wait for the measurement to complete:
delay(status);
status = pressure.getTemperature(T);
if (status != 0)
{
// Print out the measurement:
Serial.print("temperature: ");
Serial.print(T,2);
Tdeg = T;
Serial.print(" deg C, ");
Tfar = (9.0/5.0)*T+32.0;
Serial.print((9.0/5.0)*T+32.0,2);
Serial.println(" deg F");
 
status = pressure.startPressure(3);
if (status != 0)
{
// Wait for the measurement to complete:
delay(status);
status = pressure.getPressure(P,T);
if (status != 0)
{
// Print out the measurement:
Serial.print("absolute pressure: ");
Serial.print(P,2);
pmb = P;
Serial.print(" mb, ");
phg = P*0.0295333727;
Serial.print(P*0.0295333727,2);
Serial.println(" inHg");
 
p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
Serial.print("relative (sea-level) pressure: ");
Serial.print(p0,2);
Serial.print(" mb, ");
Serial.print(p0*0.0295333727,2);
Serial.println(" inHg");
 
a = pressure.altitude(P,p0);
Serial.print("computed altitude: ");
Serial.print(a,0);
Serial.print(" meters, ");
Serial.print(a*3.28084,0);
Serial.println(" feet");
}
else Serial.println("error retrieving pressure measurement\n");
}
else Serial.println("error starting pressure measurement\n");
}
else Serial.println("error retrieving temperature measurement\n");
}
else Serial.println("error starting temperature measurement\n");
 
 humidity = dht.getHumidity();
temperature = dht.getTemperature();
 
Serial.print("H:");
Serial.println(humidity);
Serial.print("T:");
Serial.println(temperature); //dht.toFahrenheit(temperature));
 
int rain = analogRead(A0);
 Serial.print("R:");
Serial.println(rain);
  int rainDigitalVal = digitalRead(rainDigital);
  String rainstatus;
  if(rainDigitalVal==1){
    rainstatus="No Rain";
    }
      else{
    rainstatus="Raining";
    }
 
//Create JSON data
String data = "{\"Rain\":\""+String(rainstatus)+"\",\"Pressuremb\":\""+String(pmb)+"\",
\"Altitude\":\""+String(a)+"\", \"Temperature\":\""+ String(temperature) +"\", 
\"Humidity\":\""+ String(humidity) +"\"}";
 
//digitalWrite(LED,!digitalRead(LED)); //Toggle LED on data request ajax
server.send(200, "text/plane", data); //Send ADC value, temperature and humidity JSON to client ajax request
 
//delay(dht.getMinimumSamplingPeriod());
 
 
}
 
void setup()
{
Serial.begin(115200);
Serial.println();
 
// dht11 Sensor
 
dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
pinMode(LED,OUTPUT);
pinMode(rainDigital,INPUT);
 
//BMP180 Sensor
if (pressure.begin())
Serial.println("BMP180 init success");
else
{
Serial.println("BMP180 init fail\n\n");
while(1); // Pause forever.
}
 
WiFi.begin(ssid, password); //Connect to your WiFi network
Serial.println("");
 
// Wait for connection
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
 
//If connection successful show IP address in serial monitor
Serial.println("");
Serial.print("Connected to ");
Serial.println(ssid);
Serial.print("IP address: ");
Serial.println(WiFi.localIP()); //IP address assigned to your ESP
 
server.on("/", handleRoot); //Which routine to handle at root location. This is display page
server.on("/readADC", handleADC); //This page is called by java Script AJAX
 
server.begin(); //Start server
Serial.println("HTTP server started");
}
 
void loop()
{
server.handleClient(); //Handle client requests
}
