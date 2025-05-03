//  https://youtu.be/oDEFAlp1Yho
//  https://www.tinkercad.com/things/i4KqjSBKx5y-copy-of-arduino-water-level-sensor/editel
//  https://youtu.be/CmkoEHnXWJQ



// These constants won't change:
const int LOWLEVEL = D1;    // pin that the sensor is attached to LOWLEVEL SENSOR
const int MIDLEVEL = D2;    // pin that the sensor is attached to MIDLEVEL SENSOR
const int HILEVEL = D3;     // pin that the sensor is attached to HILEVEL SENSOR

const int LOWLEVELLED = D5 ;
const int MIDLEVELLED = D6 ;
const int HILEVELLED = D7 ;

int LowSts = 0 ;
int MidSts = 0 ;
int HiSts  = 0 ;

void setup() {

  


  pinMode(LOWLEVEL, INPUT_PULLUP);    // Sensors
  pinMode(MIDLEVEL, INPUT_PULLUP);
  pinMode(HILEVEL, INPUT_PULLUP);

  pinMode(LOWLEVELLED, OUTPUT);       // LEDs
  pinMode(MIDLEVELLED, OUTPUT);
  pinMode(HILEVELLED, OUTPUT);

    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, LOW);

    digitalWrite(LOWLEVELLED, LOW);
    digitalWrite(MIDLEVELLED, LOW);
    digitalWrite(HILEVELLED, LOW);


}


void loop() {

  LowSts = digitalRead(LOWLEVEL);

  if (LowSts == LOW ) {
    digitalWrite(LOWLEVELLED, HIGH);
  } else {
    digitalWrite(LOWLEVELLED, LOW);
    
  }

  MidSts = digitalRead(MIDLEVEL);

  if (MidSts == LOW) {
    digitalWrite(MIDLEVELLED, HIGH);
  } else {
    digitalWrite(MIDLEVELLED, LOW);
  }

 HiSts = digitalRead(HILEVEL);

  if (HiSts == LOW) {
    digitalWrite(HILEVELLED, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);

  } else {
    digitalWrite(HILEVELLED, LOW);
        digitalWrite(LED_BUILTIN, LOW);

  }


}
