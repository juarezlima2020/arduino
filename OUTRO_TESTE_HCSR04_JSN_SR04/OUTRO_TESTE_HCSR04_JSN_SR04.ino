#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define trigPin D2
#define echoPin D1
 
long duration;
int distance;
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  Serial.begin(9600);
}
 
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
 
  distance = duration*0.034/2;
 
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
  display.clearDisplay();
 
  display.setTextSize(3);
  display.setCursor(10, 20);
  display.print(distance);
 
  display.setTextSize(3);
  display.setCursor(70, 20);
  display.print("CM");
 
  display.display();
}
