#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();


// Select which 'port' 
Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *Motor4 = AFMS.getMotor(4);

void setup() {
  // set up Serial library at 9600 bps
  Serial.begin(9600);          
  Serial.println("Motor Test");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  Motor1->setSpeed(255); // sets motor to max speed 
  Motor1->run(FORWARD);
  // turn on motor
  Motor1->run(RELEASE);


}

void loop() {

  Motor1->run(FORWARD);

  delay(2000);

  Motor1->run(BACKWARD);

  delay(2000);


}
