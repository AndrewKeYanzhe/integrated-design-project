// Servo Test 
// This program turns the servo 90 CW, and then 90 ACW
// Lewis Clark 11/10/22

#include <Servo.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Servo servo1;

void setup() {
  // set up Serial library at 9600 bps
  Serial.begin(9600);

  Serial.println("Servo Test");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Attach a servo to pin #10 - this is Servo 1 on the motor shield 
  servo1.attach(10);

}

void loop() {

  // Sets servo position to 90 deg
  servo1.write(90);

  delay(1000);

  // Sets servo position to 0 deg
  servo1.write(0);

  delay(1000);

}
