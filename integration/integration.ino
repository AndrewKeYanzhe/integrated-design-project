// leave initial box and turn right onto white line

//TODO: add printing of movement history

#include <Adafruit_MotorShield.h>
#include <Servo.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' - currently left = M1 and right = M2
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(4);

// Declaring variables
int default_speed = 255; //192 is not powerful enough to go on flat ground. 210 is a decent speed. 255 + full battery is needed to make turns with one wheel (255,0)
int loop_delay = 1;

bool sensor_1;
bool sensor_2;
bool sensor_3;
bool sensor_4;

bool enable_motors = 1;

bool far_left_white = false;
bool left_white = false;
bool right_white = false;
bool far_right_white = false;

//Define states
String state_0 = "in box";
String state_1 = "turning right";
String state_2 = "line following 1";
String state_3 = "stop and pick up block";
String state_4 = "turning 180 degrees";
String state_5 = "line following 2";
String state_6 = "turning left";
String state_7 = "line following 3";
String state_8 = "in box and dropping block";
String current_state_str = state_1;
int current_state = 0;


int left_motorSpeed = 255;
char left_motorDirection = NULL;
int right_motorSpeed = 255;
char right_motorDirection = NULL;

unsigned long startTime = NULL;

//Trigpin and echopins might need to be swapped around
#define trigPin 4
#define echoPin 5
long duration;
int ultrasound_dist;
Servo left_servo;
Servo right_servo;

//variables to change - ULTRASOUND
int contact_distance = 10;

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
  set_motor_speed('R','F',default_speed);

  Serial.println("begin going forward");
  // delay(999999);
  startTime = millis();

  //for ultrasound
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Attach a servo to pin #10 - this is Servo 1 on the motor shield 
  left_servo.attach(10);
  right_servo.attach(11); //CHANGE 
}

void read_sensors(){
  sensor_1 = digitalRead(3);
  sensor_2 = digitalRead(1);
  sensor_3 = digitalRead(2);
  sensor_4 = digitalRead(0);
 
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
  Serial.print(current_state_str);  
}

void loop() {
  // put your main code here, to run repeatedly:
  read_sensors();
  check_state();
}

void check_state(){
  if (current_state == 1){
    turn_right();
  }
  if (current_state == 2){
    follow_line();
  }
  if (current_state == 3){
    stop_motors();
    block_pick_up();
  }
  if (current_state == 4){
    turn_back();
  }
  if (current_state == 5){
    follow_line();
  }
  if (current_state == 6){
    turn_left();
  }
  if (current_state == 7){
    follow_line();
  }
  if (current_state == 8){
    stop_motors();
    block_put_down();
  }
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
    current_state = ++current_state;
    check_state();
    // turn_right(); //TODO: uncomment when using current_state_str
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
  // Transition from state 5 to 6.
   else if (far_left_white && left_white){
   current_state = ++current_state;
   check_state();
  }
  if (!left_white && !right_white && !far_left_white && !far_right_white){
   current_state = ++current_state;
   check_state();
  }
  //MAY NEED TO BE CORRECTED TO AVOID SETTING OFF WHEN GOING OVER RAMP. Transition from state 2 to 3.
  //ultrasound_dist = read_ultrasound();
  if (ultrasound_dist < contact_distance){
    current_state = ++current_state;
    check_state();
  }
  delay(loop_delay);
}

void stop_motors(){
    set_motor_speed('L','F',0);
    set_motor_speed('R','F',0);
}

// Function for initial right turn
void turn_right(){
  //loops
  read_sensors();
  set_motor_speed('L','F',255);
  set_motor_speed('R','F',default_speed*0.3);

  // stop_motors();
  // set_motor_speed('L','F',default_speed);

  if (left_white || right_white){
    Serial.println("found line");
    set_motor_speed('L','F',default_speed*0.8);
    set_motor_speed('R','F',default_speed);
    current_state = ++current_state;
    check_state();
    delay(500);
    return;
  }
}
// Function for final left turn
void turn_left(){
  //loops
  read_sensors();
  set_motor_speed('L','F', default_speed*0.3);
  set_motor_speed('R','F', 255);

  // stop_motors();
  // set_motor_speed('L','F',default_speed);

  if (left_white || right_white){
    Serial.println("found line");
    set_motor_speed('L','F',default_speed);
    set_motor_speed('R','F',default_speed);
    current_state = ++current_state;
    check_state();
    delay(10);
    return;
  }
}
void turn_back(){
  //loops
  read_sensors();
  set_motor_speed('L','F', 255);
  set_motor_speed('R','F', 0);
  if (left_white || right_white){
    Serial.println("found line");
    set_motor_speed('L','F',default_speed);
    set_motor_speed('R','F',default_speed);

    delay(10);
    return;
    current_state = ++current_state;
    check_state();
  }
}

int read_ultrasound(){
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting th1234e trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  // Calculate the ultrasound_dist:
  ultrasound_dist = duration * 0.034 / 2;

  // Print the ultrasound_dist on the Serial Monitor (Ctrl+Shift+M):
  Serial.print("Distance = ");
  Serial.print(ultrasound_dist);
  Serial.println(" cm");

  return ultrasound_dist;
}

void block_pick_up(){
  ultrasound_dist = read_ultrasound();
  if (ultrasound_dist > contact_distance){
    Serial.println("block picked up OR ultrasound_dist is far");
  }
  else {
    left_servo.write(-1);
    right_servo.write(1);
  }
  current_state = ++current_state;
  check_state();
}

void block_put_down(){
  ultrasound_dist = read_ultrasound();
  if (ultrasound_dist > contact_distance){
  Serial.println("block picked up OR ultrasound_dist is far");
  }
  else {
    left_servo.write(1);
    right_servo.write(-1);
  current_state = 0;
  }
}
