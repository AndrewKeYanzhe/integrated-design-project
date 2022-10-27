// leave initial box and turn right onto white line


#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

//--variables to set--------------------------



// Select which 'port' - currently left = M1 and right = M2
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(4);

// Declaring variables
int default_speed = 255; 
int loop_delay = 1;



//--variables to set--------------------------

bool sensor_1;
bool sensor_2;
bool sensor_3;
bool sensor_4;

bool enable_motors = 1;

bool far_left_white = false;
bool left_white = false;
bool right_white = false;
bool far_right_white = false;

bool on_line = false;

int current_state = 0;
String current_state_v2 = "initial box";
// int right_turns = 0;

int left_motorSpeed = 0;
char left_motorDirection = NULL;
int right_motorSpeed = 0;
char right_motorDirection = NULL;

unsigned long startTime = NULL;

void set_motor_speed(char motor_label, char new_direction, int new_speed) {
  if (enable_motors == 0){
    return;
  }
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

  // turn on motor


  set_motor_speed('R','F',default_speed);

  // turn on motor


  Serial.println("begin going forward");
  // delay(999999);
  startTime = millis();



}

void(* resetFunc) (void) = 0;//declare reset function at address 0


void read_sensors(){
  sensor_1 = !(digitalRead(0));
  sensor_2 = !(digitalRead(1));
  sensor_3 = !(digitalRead(2));
  sensor_4 = !(digitalRead(3));

  if (digitalRead(13) == HIGH){
    resetFunc();
  }
 
  
  far_left_white = sensor_1 == LOW;
  left_white = sensor_2 == LOW;
  right_white = sensor_3 == LOW;
  far_right_white = sensor_4 == LOW;

  unsigned long timeElapsed = millis() - startTime;
  Serial.println(" ");
  Serial.print("white: ");
  Serial.print(far_left_white);
  Serial.print(left_white);
  Serial.print(right_white);
  Serial.print(far_right_white);
  Serial.print("    motors: ");
  Serial.print(left_motorSpeed);
  Serial.print(right_motorSpeed);
  Serial.print("    time: ");
  Serial.print(timeElapsed/1000);
  Serial.print("    state: ");
  Serial.print(current_state_v2);  
  

}

void loop() {
  // put your main code here, to run repeatedly:

  read_sensors();
  

  if (left_white || right_white){
    Serial.println("found line");
    on_line = true;
    current_state_v2 = "line following";
    delay(100);
    follow_line();
  }

  delay(1);


  

}

void follow_line(){
  //loops

  read_sensors();

  if (left_white && right_white && !far_left_white && !far_right_white){
    Serial.print("    continue forward");
    set_motor_speed('L','F',default_speed);
    set_motor_speed('R','F',default_speed);
  }

  else if (far_left_white && far_right_white){
    Serial.print("    crossed white line");
    current_state = current_state + 1;
    // turn_right(); //TODO: uncomment when using current_state_V2
    
    
  }

  else if (left_white && !right_white){
    Serial.print("    slight leftward correction");
    set_motor_speed('L','F',default_speed*0.8);
    set_motor_speed('R','F',default_speed);
  }
  else if (!left_white && right_white){
    Serial.print("    slight rightward correction");
    set_motor_speed('L','F',default_speed);
    set_motor_speed('R','F',default_speed*0.8);
  }
  else if (far_left_white && !left_white && !right_white && !far_right_white){
    Serial.print("    leftward correction");
    set_motor_speed('L','F',default_speed*0);
    set_motor_speed('R','F',255);
  }
  else if (!far_left_white && !left_white && !right_white && far_right_white){
    Serial.print("    rightward correction");
    set_motor_speed('L','F',255);
    set_motor_speed('R','F',default_speed*0);
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
  if (current_state == 1){
    // stop_motors();
    turn_right();
  }

  follow_line();
  

}

void turn_right(){
  //loops
  read_sensors();
  set_motor_speed('L','F',255);
  set_motor_speed('R','F',default_speed*0.3);

  // stop_motors();

  // set_motor_speed('L','F',default_speed);


  if (left_white || right_white){
    Serial.println("found line");
    current_state = current_state+1;

    set_motor_speed('L','F',default_speed*0.8);
    set_motor_speed('R','F',default_speed);
    delay(500);
    // stop_motors();

    on_line = true;
    // right_turns = right_turns + 1;
    follow_line();
    return;
  }

  delay(loop_delay);
  turn_right();

}
