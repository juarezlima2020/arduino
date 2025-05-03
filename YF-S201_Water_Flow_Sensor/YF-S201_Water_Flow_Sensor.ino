//Coded and Tested By:
// Sheekar Banerjee, AI-ML-IOT Solution Engineer and Researcher
//https://create.arduino.cc/projecthub/sheekar/yf-s201-water-flow-sensor-and-arduino-sheekar-banerjee-853bd2?ref=part&ref_id=8233&offset=15
/*
Arduino Water flow meter
YF- S201 Hall Effect Water Flow Sensor
Water Flow Sensor output processed to read in litres/hour
*/

volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

void flow () // Interrupt function
{
flow_frequency++;
}

void setup()
{
pinMode(flowsensor, INPUT);
digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
Serial.begin(9600);
attachInterrupt(0, flow, RISING); // Setup Interrupt
 sei(); // Enable interrupts
currentTime = millis();
cloopTime = currentTime;
}

void loop ()
{
currentTime = millis();
 // Every second, calculate and print litres/hour
if(currentTime >= (cloopTime + 1000))
{
 cloopTime = currentTime; // Updates cloopTime

 // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
 l_hour = (flow_frequency * 60 / 7.5);
 // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour

 flow_frequency = 0; // Reset Counter
 Serial.print(l_hour, DEC); // Print litres/hour
 Serial.println(" L/hour");
}
}
