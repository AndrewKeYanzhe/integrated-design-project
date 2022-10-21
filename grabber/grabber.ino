// Grabber
// servo.write is clockwise

#include <Servo.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Servo left_servo;
Servo right_servo;

void setup() {
  // set up Serial library at 9600 bps
  Serial.begin(9600);
  AFMS.begin();

//   Serial.println("");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Attach a servo to pin #10 - this is Servo 1 on the motor shield 
  left_servo.attach(10);
  right_servo.attach(11); //CHANGE

}

void loop() {

  // lift up
  left_servo.write(-90);
  right_servo.write(90);

  delay(1000);



}