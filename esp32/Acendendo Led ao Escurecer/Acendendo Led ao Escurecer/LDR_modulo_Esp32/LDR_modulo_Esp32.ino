/**************************
       Usinainfo
  www.usinainfo.com.br 
  https://www.usinainfo.com.br/sensor-de-luminosidade/modulo-sensor-de-luminosidade-ldr-para-arduino-e-esp32-2539.html
 **************************/
 
const int buttonPin = 21; 
const int ledPin = 22; 
int buttonState = 0;
void setup() {
Serial.begin(115200);

pinMode(buttonPin, INPUT);

pinMode(ledPin, OUTPUT);
}

void loop() {

buttonState = digitalRead(buttonPin);
Serial.println(buttonState);

if (buttonState == HIGH) {

digitalWrite(ledPin, LOW);
}
else {

digitalWrite(ledPin, HIGH);}
}
