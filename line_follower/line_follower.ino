#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' 
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(2);
Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *Motor4 = AFMS.getMotor(4);

int default_speed = 192; //75% of max speed



bool SensorOne;
bool SensorTwo;




void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);


  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  left_motor->setSpeed(default_speed); 
  left_motor->run(FORWARD);
  // turn on motor
  left_motor->run(RELEASE);  

  right_motor->setSpeed(default_speed); 
  right_motor->run(FORWARD);
  // turn on motor
  right_motor->run(RELEASE);  



}

void loop() {
  // put your main code here, to run repeatedly:

  SensorOne = digitalRead(0);
  if (SensorOne == HIGH) {
    Serial.println("sensor 1 BLACK");
  }
  else{
    Serial.println("sensor 1 WHITE");
  }
  
  SensorTwo = digitalRead(1);

  bool left_black = SensorOne == HIGH;
  bool right_black = SensorTwo == HIGH;

  if (left_black && right_black){
    Serial.println("continue forward");
    left_motor->setSpeed(default_speed);
    right_motor->setSpeed(default_speed);
  }

  else if (left_black && !right_black){
    Serial.println("turn right");
    left_motor->setSpeed(default_speed);
    right_motor->setSpeed(default_speed*0.9);
  }

  else if (!left_black && right_black){
    Serial.println("turn left");
    left_motor->setSpeed(default_speed*0.9);
    right_motor->setSpeed(default_speed);
  }

  delay(1);

}
