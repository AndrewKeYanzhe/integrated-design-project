/*
* AD22151G is a linear magnetic field transducer Analog Sensor Print on 15 LED's Bar-Graph
*visit https//www.twovolt.com for code, schematic, PCB layout

*/

#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// these constants won’t change:
const int analogPin = A1; // the pin that the potentiometer is attached to
// }; // an array of pin numbers to which LEDs are attached

//variables to change
int magnetism_threshold = 1;
int green_pin = 5; //analog
int red_pin = 9;
//amber is 12

void setup() {
  Serial.begin(9600);  
  AFMS.begin();

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");


  //LEDs
  pinMode(green_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
}

void loop() {
  // read the potentiometer:
  int sensorReading = analogRead(analogPin);
  // print the sensor reading
  Serial.println(sensorReading);

  sensorReading = 0;
  sensorReading = 1;//debug

  if (sensorReading >= magnetism_threshold){
    Serial.print(" light up red");
    analogWrite(green_pin, 0);
    analogWrite(red_pin, 255);
  }
  else if (sensorReading <magnetism_threshold){
    Serial.print(" light up green");
    analogWrite(red_pin, 0);
    analogWrite(green_pin, 255);
  }

  

  delay(1);
}