// leave initial box and turn right onto white line

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' - currently left = M1 and right = M2
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(2);

// Declaring variables

int default_speed = 192; //75% of max speed

int loop_delay = 1;

bool sensor_1;
bool sensor_2;
bool sensor_3;
bool sensor_4;

bool far_left_black = false;
bool left_black = false;
bool right_black = false;
bool far_right_black = false;

bool on_line = false;

int white_line_crossed = 0;

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
  left_motor->run(RELEASE);  

  set_motor_speed('R',default_speed);
  right_motor->run(FORWARD);
  right_motor->run(RELEASE); 

  Serial.println("going forward");



}

void read_sensors(){
  sensor_1 = digitalRead(0);
  sensor_2 = digitalRead(1);
  sensor_3 = digitalRead(2);
  sensor_4 = digitalRead(3);

  if (sensor_2 == HIGH) {
    Serial.println("sensor 1 BLACK");
  }
  else{
    Serial.println("sensor 1 WHITE");
  }
  
  
  far_left_black = sensor_1 == HIGH;
  left_black = sensor_2 == HIGH;
  right_black = sensor_3 == HIGH;
  far_right_black = sensor_4 == HIGH;
}

void loop() {
  // put your main code here, to run repeatedly:

  read_sensors();
  
  if (left_black && right_black && far_left_black && far_right_black){
    Serial.println("found line");
    on_line = true;
    follow_line();
  }

  delay(loop_delay);
}

void follow_line(){
  //loops
  left_motor->run(FORWARD);
  left_motor->run(RELEASE); 
  right_motor->run(FORWARD);
  right_motor->run(RELEASE); 

  read_sensors();

  if (left_black && right_black && far_left_black && far_right_black){
    Serial.println("continue forward");
    set_motor_speed('L',default_speed);
    set_motor_speed('R',default_speed);
  }

  else if (!far_left_black && !far_right_black){
    Serial.println("crossed white line");
    white_line_crossed = white_line_crossed + 1;
    check_state();
  }

  else if (!left_black && right_black){
    Serial.println("nudge left");
    set_motor_speed('L',default_speed*0.9);
    set_motor_speed('R',default_speed);
  }
  else if (left_black && !right_black){
    Serial.println("nudge right");
    set_motor_speed('L',default_speed);
    set_motor_speed('R',default_speed*0.9);
  }
  

  delay(loop_delay);
  follow_line();
}

void stop_motors(){
    set_motor_speed('L',0);
    set_motor_speed('R',0);
}

void check_state(){
  Serial.println("checking state");
  if (white_line_crossed == 1){
    stop_motors();
    turn_right();

  }

}

void turn_right(){
  //loops
  read_sensors();
  Serial.println("turn right");

  stop_motors();

  set_motor_speed('L',default_speed);
  left_motor->run(FORWARD);
  left_motor->run(RELEASE); 
  /*Test factor for right wheel speed so that line sensors will line up with line after 90 degree right turn. If right wheel is stationary it will overshoot the line*/
  set_motor_speed('R',default_speed*0.2);
  right_motor->run(BACKWARD);
  right_motor->run(RELEASE);  


  if (left_black && right_black && far_left_black && far_right_black){
    Serial.println("found line");
    stop_motors();

    on_line = true;
    follow_line();
  }

  delay(loop_delay);
  turn_right();

}
