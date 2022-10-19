// leave initial box and turn right onto white line

//TODO: add printing of movement history

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' - currently left = M1 and right = M2
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(4);

// Declaring variables
int default_speed = 0; //TODO //at 64, only one motor runs. at 128, motors only run when not touching ground. 192 is not powerful enough to go on flat ground

int loop_delay = 500;

bool sensor_1;
bool sensor_2;
bool sensor_3;
bool sensor_4;

bool far_left_white = false;
bool left_white = false;
bool right_white = false;
bool far_right_white = false;

bool on_line = false;

int white_line_crossed = 0;

int left_motorSpeed = 0;
char left_motorDirection = NULL;
int right_motorSpeed = 0;
char right_motorDirection = NULL;

unsigned long startTime = NULL;

void set_motor_speed(char motor_label, char new_direction, int new_speed) {
  //read old speed
  int old_speed = NULL;
  char old_direction = NULL;

  switch(motor_label){
    case 'L':
      old_speed = left_motorSpeed;
      old_direction = left_motorDirection;

      break;
    case 'R':
      old_speed = right_motorSpeed;
      old_direction = right_motorDirection;

      break;
  }

  //set new speed
  if (new_speed==old_speed && new_direction==old_direction){
    return;
  }
  else {
    // send a command that sets the  motor speed to the new speed, then ...
    switch(motor_label){
    case 'L':
      left_motor->setSpeed(new_speed);
      left_motorSpeed = new_speed;

      switch(new_direction){
        case 'F':
          left_motor->run(FORWARD);
          left_motorDirection = new_direction;
        case 'B':
          left_motor->run(BACKWARD);
          left_motorDirection = new_direction;
      }      
      break;
    case 'R':
      Serial.println("setting right speed");
      right_motor->setSpeed(new_speed);
      right_motorSpeed = new_speed;
      


      switch(new_direction){
        case 'F':
          right_motor->run(FORWARD);
          Serial.println("running right");
          right_motorDirection = new_direction;
        case 'B':
          right_motor->run(BACKWARD);
          right_motorDirection = new_direction;
      }      
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

  set_motor_speed('L','F',default_speed);
  // left_motor->run(FORWARD);
  // turn on motor


  set_motor_speed('R','F',default_speed);
  // right_motor->run(FORWARD);
  // turn on motor


  Serial.println("begin going forward");
  // delay(999999);
  startTime = millis();



}

void read_sensors(){
  sensor_1 = digitalRead(0);
  sensor_2 = digitalRead(1);
  sensor_3 = digitalRead(2);
  sensor_4 = digitalRead(3);
 
  
  far_left_white = sensor_1 == LOW;
  left_white = sensor_2 == LOW;
  right_white = sensor_3 == LOW;
  far_right_white = sensor_4 == LOW;

  unsigned long timeElapsed = millis() - startTime;
  Serial.println(" ");
  Serial.print("sensing white: ");
  Serial.print(far_left_white);
  Serial.print(left_white);
  Serial.print(right_white);
  Serial.print(far_right_white);
  Serial.print("    motor speeds: ");
  Serial.print(left_motorSpeed);
  Serial.print(right_motorSpeed);
  Serial.print("    time elapsed: ");
  Serial.print(timeElapsed/1000);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  read_sensors();
  

  if (left_white && right_white && !far_left_white && !far_right_white){
    Serial.println("found line");
    on_line = true;
    follow_line();
  }


  delay(loop_delay);

}

void follow_line(){
  //loops

  read_sensors();

  if (left_white && right_white && !far_left_white && !far_right_white){
    Serial.print("    continue forward");
    set_motor_speed('L','F',default_speed);
    set_motor_speed('R','F',default_speed);
  }

  else if (left_white && right_white && far_left_white && far_right_white){
    Serial.print("    crossed white line");
    white_line_crossed = white_line_crossed + 1;
    
  }

  else if (left_white && !right_white){
    Serial.print("    nudge left");
    set_motor_speed('L','F',default_speed*0.9);
    set_motor_speed('R','F',default_speed);
  }
  else if (!left_white && right_white){
    Serial.print("    nudge right");
    set_motor_speed('L','F',default_speed);
    set_motor_speed('R','F',default_speed*0.9);
  }

  check_state();
  

  delay(loop_delay);
  follow_line();
}

void stop_motors(){
    set_motor_speed('L','F',0);
    set_motor_speed('R','F',0);
}

void check_state(){
  // Serial.println("checking state");
  // if (white_line_crossed == 1){
  //   stop_motors();
  //   turn_right();
  // }

  follow_line();
  

}

void turn_right(){
  //loops
  read_sensors();
  Serial.println("turn right");

  stop_motors();

  set_motor_speed('L','F',default_speed);

  if (left_white && right_white && !far_left_white && !far_right_white){
    Serial.println("found line");
    stop_motors();

    on_line = true;
    follow_line();
  }

  delay(loop_delay);
  turn_right();

}
