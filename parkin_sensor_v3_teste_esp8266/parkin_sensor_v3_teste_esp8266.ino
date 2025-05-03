#include "LowPower.h"

#include <Adafruit_NeoPixel.h>

#define N_LEDS      16    //adjust this number to reflect the number of LEDs in your LED strip
#define PIN         D3
#define LED         D4
const int trigPin = D7;
const int echoPin = D5;

int offThreshhold =   200; //turn lights to red    (Vermelho - 200)90  -140 
int closeThreshhold = 150; //turnlights to yellow  (Amarelo  - 180)120 -110 
int safeThreshhold =  100; //turn lights to green  (Verde    - 120)180 - 80
int tooClose =         50; //turn lights to purple (Roxo     -  90)150 - 50
int stop =             20; //flashes warning lights(Pisca    -  60)60  - 20

int wait = 10*1000;        //#of seconds to wait before turning off 
                           //(multiplied by 1000 to convert to milliseconds)
float gap = 1.5;           //noise threshold for detecting movement

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t white = strip.Color(255, 255, 255); //020white = strip.Color(255, 255, 255);
uint32_t off = strip.Color(0, 0, 0);         //020off = strip.Color(0, 0, 0);
uint32_t bG = strip.Color(15, 255, 200);     //bG = strip.Color(15, 255, 200);
uint32_t purple = strip.Color(255, 0, 0);  //purple = strip.Color(200, 5, 150);
uint32_t red = strip.Color(255, 5, 15);       //red = strip.Color(255, 0, 0);  
uint32_t green = strip.Color(255, 255, 0);     //green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);      //blue = strip.Color(0, 0, 255); 
uint32_t yellow = strip.Color(0, 255, 0);  //yellow = strip.Color(170, 200, 0);

long duration;
float distancecm;
float oldDistance = 0;

unsigned long changeTime;

boolean noChange = false;
boolean sleepTime = false;
boolean even = true;
int middle = 0;
int range;
int adj = 1;
int adj2 = 0;

void setup() {
  pinMode (trigPin, OUTPUT);
  pinMode (echoPin, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  middle = N_LEDS / 2;
  if (N_LEDS % 2 == 1) {
    even = false;
  }
  if (even == true) {
    adj = 1;
    adj2 = 0;
  } else {
    adj = 0;
    adj2 = 1;
  }
  Serial.begin(9600);
  strip.begin();

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distancecm = duration * .0342 / 2;
  Serial.println(distancecm);
  delay(200);
  sleep();
 
}

void sleep(){
 
  if (distancecm >= oldDistance) {
    if (distancecm - oldDistance <= gap) {
      noChange = true;
    } else {
      noChange = false;
    }
  } else {
    if (oldDistance - distancecm <= gap) {
      noChange = true;
    } else {
      noChange = false;
    }
  }
  if (noChange == true) {
    if (millis() - changeTime > wait) {
      sleepTime = true;
    }
  } else {
    sleepTime = false;
  }

  if (distancecm > offThreshhold || sleepTime == true) {
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, off);
    }
    strip.show();
  //   LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
  } else {
    colorChange2();
    if (noChange == false) {
      changeTime = millis();
      oldDistance = distancecm;
    }

  }

}

void colorChange2() {

  if (distancecm <= offThreshhold && distancecm > closeThreshhold) {
    range = map(distancecm, offThreshhold, closeThreshhold,  0, middle+adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, off);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, red);
      strip.setPixelColor(middle - adj -i, red);
    }

  } else if (distancecm <= closeThreshhold && distancecm > safeThreshhold) {
    range = map(distancecm, closeThreshhold, safeThreshhold, 0, middle+adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, red);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, yellow);
      strip.setPixelColor(middle - adj -i, yellow);
    }
  } else if (distancecm <= safeThreshhold && distancecm > tooClose) {
    range = map(distancecm, safeThreshhold, tooClose, 0, middle+adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, yellow);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, green);
      strip.setPixelColor(middle - adj -i, green);
    }
  } else if (distancecm <= tooClose && distancecm > stop) {
    range = map(distancecm, tooClose, stop, 0, middle+adj2);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, green);
    }
    for (int i = 0; i < range; i++) {
      strip.setPixelColor(middle + i, purple);
      strip.setPixelColor(middle - adj -i, purple);
    }
  } else if(distancecm<stop){
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, white);
    }
    strip.show();
    delay(300);
    for (int i = 0; i < N_LEDS; i++) {
      strip.setPixelColor(i, red);
    }
    strip.show();
    delay(300);
  }
  strip.show();
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
