// leave initial box and turn right onto white line

bool enable_motors = 1;

// enable_motors = 0;

#include <Adafruit_MotorShield.h>
#include <Servo.h>

// Define Trig and Echo pin for ultrasound
#define trigPin_left 7
#define echoPin_left 6
#define trigPin_front 5
#define echoPin_front 4

Servo left_servo;
Servo right_servo;



const int hallEffectPin = 8;
int hall_sensor = NULL;

bool stopped = 0;
bool holding_block = 0;
bool magnetic = 0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Define variables for ultrasound:
long left_ultrasound_duration;
int left_dist;
long front_ultrasound_duration;
int front_dist;

//--variables to set--------------------------

int amber_pin = 12;

// Select which 'port' - currently left = M1 and right = M2
Adafruit_DCMotor *left_motor = AFMS.getMotor(4);
Adafruit_DCMotor *right_motor = AFMS.getMotor(1);

// Declaring variables
int default_speed = 255; 
int loop_delay = 1;



//--variables to set--------------------------

bool sensor_1;
bool sensor_2;
bool sensor_3;
bool sensor_4;

int tjunctions_crossed = 0;
unsigned long tjunction_timestamp = NULL;

bool far_left_white = false;
bool left_white = false;
bool right_white = false;
bool far_right_white = false;

bool on_line = false;


String current_state_v2 = "initial box";
// int right_turns = 0;

int left_motorSpeed = 0;
char left_motorDirection = NULL;
int right_motorSpeed = 0;
char right_motorDirection = NULL;

unsigned long startTime = NULL;
unsigned long begin_stopping = NULL;

float left_motor_ratio = 0.95;

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
      left_motor->setSpeed(new_speed*left_motor_ratio);
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
      right_motor->setSpeed(new_speed);
      right_motorSpeed = new_speed;
      


      switch(new_direction){
        case 'F':
          right_motor->run(FORWARD);
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

  // Define inputs and outputs for ultrasound:
  pinMode(trigPin_left, OUTPUT);
  pinMode(echoPin_left, INPUT);
  pinMode(trigPin_front, OUTPUT);
  pinMode(echoPin_front, INPUT);

  Serial.begin(9600);
  Serial.println("starting setup");
  AFMS.begin();
  Serial.println("started AFMS");


  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // while (true){
    
  //   if (digitalRead(13) == HIGH){
  //     delay(100);
  //     break;
  //   }
  // }

  left_servo.attach(10);
  right_servo.attach(9);

  pinMode (amber_pin, OUTPUT);

  // while (digitalRead(12) == LOW){
  //   delay(1);
  // }

  //driving position
  left_servo.write(0);
  right_servo.write(180); 



  set_motor_speed('L','F',default_speed);
  set_motor_speed('R','F',default_speed);


  // Serial.println("begin going forward");
  startTime = millis();



}

void(* resetFunc) (void) = 0;//declare reset function at address 0


void read_sensors(){
  sensor_1 = !(digitalRead(0));
  sensor_2 = !(digitalRead(1));
  sensor_3 = !(digitalRead(2));
  sensor_4 = !(digitalRead(3));

  // if (digitalRead(12) == HIGH){
    // resetFunc();
  // }

  if (left_motorSpeed + right_motorSpeed > 0){
    digitalWrite(amber_pin,((millis()-startTime)/250)%2);
  }
  
  far_left_white = sensor_1 == LOW;
  left_white = sensor_2 == LOW;
  right_white = sensor_3 == LOW;
  far_right_white = sensor_4 == LOW;

  //note: if ultrasound sensors are not attached, this part of code will run very slowly. each loop takes 1-2 seconds

  //read left ultrasound sensor
  // Clear the trigPin_left by setting it LOW:
  digitalWrite(trigPin_left, LOW);
  delayMicroseconds(15);
  // Trigger the sensor by setting th1234e trigPin_left high for 10 microseconds:
  digitalWrite(trigPin_left, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin_left, LOW);
  // Read the echoPin_left, pulseIn() returns the left_ultrasound_duration (length of the pulse) in microseconds:
  left_ultrasound_duration = pulseIn(echoPin_left, HIGH);
  // Calculate the distance:
  left_dist = left_ultrasound_duration * 0.034 / 2;

  //read front ultrasound sensor
  // Clear the trigPin_front by setting it LOW:
  digitalWrite(trigPin_front, LOW);
  delayMicroseconds(15);
  // Trigger the sensor by setting th1234e trigPin_left high for 10 microseconds:
  digitalWrite(trigPin_front, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin_front, LOW);
  // Read the echoPin_left, pulseIn() returns the left_ultrasound_duration (length of the pulse) in microseconds:
  front_ultrasound_duration = pulseIn(echoPin_front, HIGH);
  // Calculate the distance:
  front_dist = front_ultrasound_duration * 0.034 / 2;

  hall_sensor = !digitalRead(hallEffectPin);


  unsigned long timeElapsed = millis() - startTime;
  Serial.println(" ");

  //line sensor
  Serial.print("white: ");
  Serial.print(far_left_white);
  Serial.print(left_white);
  Serial.print(right_white);
  Serial.print(far_right_white);

  //motor
  Serial.print("    motors: ");
  Serial.print(left_motorSpeed);
  Serial.print(", ");
  Serial.print(right_motorSpeed);

  //ultrasound
  Serial.print("    left: ");
  Serial.print(left_dist);
  Serial.print("    front: ");
  Serial.print(front_dist);

  //hall sensor
  Serial.print("    magnetic: ");
  Serial.print(hall_sensor);


  //time  
  Serial.print("    time: ");
  Serial.print(timeElapsed/1000);

  Serial.print("    tjunc: ");
  Serial.print(tjunctions_crossed);

  //state
  Serial.print("    state: ");
  Serial.print(current_state_v2);  
  
  

}

void loop() {
  Serial.println("running loop");
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

void pick_up(){
  read_sensors();
  magnetic = hall_sensor;

  int angle = 80;

  

  delay(5000);
  for (int i=0;i<=angle;i++){
    left_servo.write(90+i);
    right_servo.write(90-i);
    delay(100);
  }
  holding_block = 1;
  delay(9999999999);

}

void follow_line(){
  //loops

  read_sensors();

  //stop in front of block. robot is still able to go up the ramp
  //distance of 9 is 5cm. distance needs to be <=2 to detect magnetism
  if (front_dist <=9 and stopped == 0){
    set_motor_speed('L','F',default_speed);
    set_motor_speed('R','F',default_speed);

    if (begin_stopping == NULL){
      begin_stopping = millis();
    }

    if (millis() - begin_stopping >=750){ //700 is good
      stop_motors();
      stopped = 1;
      pick_up();      
    }
    
    // delay(800);
    // stop_motors();
    // stopped = 1;
    
    // delay(99999999);    

  }

  if (stopped){
    follow_line();
  }


  // finding right T junction
  if((left_white || right_white) && far_right_white){
    if (tjunction_timestamp == NULL){
      tjunction_timestamp = millis();
      tjunctions_crossed = tjunctions_crossed + 1;
    }
    else if (millis()-tjunction_timestamp > 1000){
      tjunctions_crossed = tjunctions_crossed + 1;
    }
  }

  if (left_white && right_white && !far_left_white && !far_right_white){
    Serial.print("    continue forward");
    set_motor_speed('L','F',default_speed);
    set_motor_speed('R','F',default_speed);
  }


  //crossing white line
  else if (far_left_white && far_right_white){
    Serial.print("    crossed white line");

    delay(100);

    turn_right(); //TODO: uncomment when using current_state_V2
    
    
  }

  //normal course correction
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

  //course correction in tunnel (all sensors read black)
  else if (far_left_white + left_white + right_white + far_right_white ==0){
    if (left_dist >=0 && left_dist <6){
      Serial.print("    slight rightward correction");
      set_motor_speed('L','F',default_speed);
      set_motor_speed('R','F',default_speed*0.8);    
    }
    else if (left_dist <= 15 && left_dist>6){
      Serial.print("    slight leftward correction");
      set_motor_speed('L','F',default_speed*0.8);
      set_motor_speed('R','F',default_speed);    
    }
  }


  

  delay(loop_delay);
  follow_line();
}

void stop_motors(){
    set_motor_speed('L','F',0);
    set_motor_speed('R','F',0);
}



void turn_right(){
  current_state_v2 = "turning right";

  //loops
  read_sensors();
  set_motor_speed('L','F',255);
  set_motor_speed('R','F',default_speed*0.3);


  if (left_white || right_white){
    Serial.println("found line");
    current_state_v2 = "line following";


    set_motor_speed('L','F',default_speed*0.8);
    set_motor_speed('R','F',default_speed);
    delay(500);
    // stop_motors();

    on_line = true;
    follow_line();
    return;
  }

  delay(loop_delay);
  turn_right();

}
