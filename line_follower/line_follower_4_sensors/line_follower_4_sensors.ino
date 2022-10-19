#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' - currently left = M1 and right = M2
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(2);

// Declaring variables

int default_speed = 192; //75% of max speed

bool sensor_1;
bool sensor_2;
bool sensor_3;
bool sensor_4;

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
  AFMS.begin();


  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  set_motor_speed('L',default_speed);
  left_motor->run(FORWARD);
  // left_motor->run(RELEASE);  

  set_motor_speed('R',default_speed);
  right_motor->run(FORWARD);
  // right_motor->run(RELEASE);  



}

void loop() {
  // put your main code here, to run repeatedly:
    set_motor_speed('L',default_speed);
    left_motor->run(FORWARD);
    // left_motor->run(RELEASE);  
    
    set_motor_speed('R',default_speed);
    right_motor->run(FORWARD);
    // right_motor->run(RELEASE);  

  sensor_2 = digitalRead(0);
  if (sensor_2 == HIGH) {
    Serial.println("sensor 1 BLACK");
  }
  else{
    Serial.println("sensor 1 WHITE");
  }
  
  sensor_3 = digitalRead(1);

  bool far_left_black = sensor_1 == HIGH;
  bool left_black = sensor_2 == HIGH;
  bool right_black = sensor_3 == HIGH;
  bool far_right_black = sensor_4 == HIGH;

  if (left_black && right_black){
    Serial.println("continue forward");
  }

  else if (!left_black && right_black){
    Serial.println("nudge left");
    set_motor_speed('L',default_speed*0.9);
  }
  else if (left_black && !right_black){
    Serial.println("nudge right");
    set_motor_speed('R',default_speed*0.9);
  }
  if(!far_left_black && !far_right_black){
    Serial.println("white cross reached")
    stop_motors()
    //CV then used to determine location of block and robot turns a certain angle
  delay(1);

}
void stop_motors(){
    set_motor_speed('L',0);
    set_motor_speed('R',0);
}
void turn_back(){
  //loops
  read_sensors();
  Serial.println("turn angle");

  set_motor_speed('R',default_speed);
  right_motor->run(FORWARD);
  // right_motor->run(RELEASE); 
  
  set_motor_speed('L',default_speed);
  left_motor->run(BACKWARD);
  // left_motor->run(RELEASE); 

  if (left_white && right_white && !far_left_white && !far_right_white){
    Serial.println("found line");
    stop_motors();

    on_line = true;
    follow_line();
  }