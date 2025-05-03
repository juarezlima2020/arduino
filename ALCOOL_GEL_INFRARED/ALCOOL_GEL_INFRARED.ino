//https://fabricreator.wixsite.com/fabricreator/dispensador-automatico

#include <Servo.h>

Servo tap_servo;

int sensor_pin = 5;
int tap_servo_pin =11;
int val;

void setup(){
  pinMode(sensor_pin,INPUT);
  pinMode(7, OUTPUT);
  tap_servo.attach(tap_servo_pin);
  
  
}

void loop(){
  val = digitalRead(sensor_pin);

  if (val==0)
  {tap_servo.write(160);
  digitalWrite(7, HIGH);
  delay(1000);
  
  }
  if (val==1)
  {tap_servo.write(0);
  digitalWrite(7, LOW);
  delay(1000);
    }
}
