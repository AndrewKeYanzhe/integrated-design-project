/*
* AD22151G is a linear magnetic field transducer Analog Sensor Print on 15 LED's Bar-Graph
*visit https//www.twovolt.com for code, schematic, PCB layout

*/

// these constants won’t change:
const int analogPin = A1; // the pin that the potentiometer is attached to
}; // an array of pin numbers to which LEDs are attached

void setup() {
  Serial.begin(9600);  
  AFMS.begin();

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");
}

void loop() {
  // read the potentiometer:
  int sensorReading = analogRead(analogPin);
  // print the sensor reading
  Serial.println(sensorReading)
  delay(100)
}