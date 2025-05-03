//https://www.youtube.com/watch?v=XkWkTyl2cz8&ab_channel=SilvasianMahen

int irsensor=2; // THE OUTPUT OF IR SENSOR CONNECTED TO PIN NO 10.
int relay_module=0; // THE RELAY MODULE CONNECTED TO PIN NO 13.
int sensor_value;  // VARIABLE TO HOLD READ SENSOR VALUE.


void setup() {
pinMode(irsensor,INPUT); // CONFIGURING PIN NO 10 AS INPUT.
pinMode(relay_module,OUTPUT); //CONFIGURING PIN NO 13 AS OUTPUT.
Serial.begin(9600); // SHOWS OUTPUT VALUE OF SENSOR IN SERIAL MONITOR.

}

void loop() {
  sensor_value=digitalRead(irsensor);
  if(sensor_value==HIGH)
  {
    digitalWrite(relay_module,HIGH);
    Serial.println("Obstacle Detected");
    }
    else
    {
      digitalWrite(relay_module,LOW);
      Serial.println("No Obstacle Detected");
      }

}
