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

int left_motorSpeed = 0;
int right_motorSpeed = 0;

void set_motor_speed(char motor_label,int new_speed) {
  //read old speed
  int old_speed = NULL;
  switch(motor_label){
    case 'L':
      old_speed = left_motorSpeed;
      break;
    case 'R':
      old_speed = right_motorSpeed;
      break;
  }

  //set new speed
  if (new_speed==old_speed){
    return;
  }
  else {
    // send a command that sets the  motor speed to the new speed, then ...
    switch(motor_label){
    case 'L':
      left_motor->setSpeed(new_speed);
      left_motorSpeed = new_speed;
      break;
    case 'R':
      right_motor->setSpeed(new_speed);
      right_motorSpeed = new_speed;
      break;
    }
  }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);


  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  set_motor_speed('L',default_speed);
  left_motor->run(FORWARD);
  // turn on motor
  left_motor->run(RELEASE);  

  set_motor_speed('R',default_speed);
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
    set_motor_speed('L',default_speed);
    set_motor_speed('R',default_speed);
  }

  else if (left_black && !right_black){
    Serial.println("turn right");
    set_motor_speed('L',default_speed);
    set_motor_speed('R',default_speed*0.9);
  }
  else if (!left_black && right_black){
    Serial.println("turn left");
    set_motor_speed('L',default_speed*0.9);
    set_motor_speed('R',default_speed);
  }

  delay(1);

}
