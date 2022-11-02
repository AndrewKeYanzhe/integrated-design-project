// leave initial box and turn right onto white line

bool enable_motors = 1;
bool debug_return = 1;

//Wednesday

#include <Adafruit_MotorShield.h>
#include <Servo.h>

// Define Trig and Echo pin for ultrasound
#define trigPin_left 7
#define echoPin_left 6
#define trigPin_front 5
#define echoPin_front 4

//grabbers
Servo left_servo;
Servo right_servo;



const int hallEffectPin = 8;
int hall_sensor = NULL;

bool stopped = 0;
bool holding_block = 0;
bool magnetic = 0;
bool white_square_on_left = 0;
bool white_square_on_right = 0;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Define variables for ultrasound:
long left_ultrasound_duration;
int left_dist;
long front_ultrasound_duration;
int front_dist;

//--variables to set--------------------------

int amber_pin = 13;
int green_pin = 12;
int red_pin = 11; 

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

int right_tjunctions_crossed = 0;
unsigned long right_tjunction_timestamp = NULL;


bool far_left_white = false;
bool left_white = false;
bool right_white = false;
bool far_right_white = false;



String current_state = "initial box";
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
  
  int old_speed = NULL;
  char old_direction = NULL;

  //read old speed
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
            left_motor->run(BACKWARD);      
            break;
          case 'B':
            left_motor->run(FORWARD);
            break;
        }
        left_motorDirection = new_direction;
        break;
      case 'R':
        right_motor->setSpeed(new_speed);
        right_motorSpeed = new_speed;
        switch(new_direction){
          case 'F':
            right_motor->run(BACKWARD);
            break;
          case 'B':
            right_motor->run(FORWARD);
            break;
        
        }
        right_motorDirection = new_direction;
        break;     
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

  pinMode(green_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode (amber_pin, OUTPUT);
  digitalWrite(green_pin, 0);
  digitalWrite(red_pin, 0);
  digitalWrite(amber_pin, 0);

  //start button //change to analog 5
  while (analogRead(0) <500){
    delay(1);
    // Serial.println("waiting for start button press");
    read_sensors();
  }

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
  //print outputs
  sensor_1 = !(digitalRead(0));
  sensor_2 = !(digitalRead(1));
  sensor_3 = !(digitalRead(2));
  sensor_4 = !(digitalRead(3));

  // if (digitalRead(12) == HIGH){
    // resetFunc();
  // }
  if (analogRead(0)>500 && millis() - startTime > 1000){
    delay(500);
    resetFunc();
  }

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

  Serial.print("    right_tjunc: ");
  Serial.print(right_tjunctions_crossed);

  //state
  Serial.print("    state: ");
  Serial.print(current_state);  

  // Serial.print(analogRead(0));
  
  

}

void loop() {
  // Serial.println("running loop");
  // put your main code here, to run repeatedly:

  read_sensors();
  

  if (left_white || right_white){
    Serial.println("found line");
    current_state = "line following";
    delay(100);
    follow_line();
  }

  delay(1);


  

}

void pick_up(){
  read_sensors();
  magnetic = magnetic + hall_sensor;

  if (magnetic){
    digitalWrite(red_pin, 1);
  }
  else {
    digitalWrite(green_pin, 1);
  }
  

  int angle = 80;

  

  delay(5000);

  left_servo.write(90);
  right_servo.write(90);
  for (int i=0;i<=angle;i++){
    left_servo.write(90+i);
    right_servo.write(90-i);
    delay(100);
  }
  holding_block = 1;
  right_tjunctions_crossed = 0;
  // delay(9999999999); //debug

  delay(1000);
  set_motor_speed('L','F',255);
  set_motor_speed('R','F',default_speed*0);

  delay(12000);

  stopped = 0;
  set_motor_speed('L','F',default_speed);
  set_motor_speed('R','F',default_speed);
  follow_line();


}

void follow_line(){
  //loops
  current_state = "line_following";

  read_sensors();

  //stop in front of block. robot is still able to go up the ramp
  //distance of 9 is 5cm. distance needs to be <=2 to detect magnetism
  if (front_dist <=9 and stopped == 0 && holding_block ==0){
    set_motor_speed('L','F',default_speed);
    set_motor_speed('R','F',default_speed);

    if (begin_stopping == NULL){
      
      stop_motors();
      delay(1000);
      set_motor_speed('L','F',default_speed);
      set_motor_speed('R','F',default_speed);
      begin_stopping = millis();
    }

    //decellerate
    if (millis() - begin_stopping >=850){ //700 is good
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

  // finding left T junction
  if((left_white || right_white) && far_left_white){
    if(white_square_on_left){
      enter_square_on_left();
      delay(999999999999);
    }
  }

  if (debug_return){
    holding_block = 1; //debug turn off
  }


  //dropping block
  // finding right T junction
  if((left_white || right_white) && far_right_white){
    if (right_tjunction_timestamp == NULL){
      right_tjunction_timestamp = millis();
      right_tjunctions_crossed = right_tjunctions_crossed + 1;
    }
    else if (millis()-right_tjunction_timestamp > 1000){
      right_tjunctions_crossed = right_tjunctions_crossed + 1;
      right_tjunction_timestamp = millis();
    }

    //right T junction found
    if (white_square_on_right){
      enter_square_on_right();

      set_motor_speed('L','F',255);
      set_motor_speed('R','F',255);  
      delay(500);
      stop_motors();
      delay(99999999);
    }
    //magnetic return to red sqaure, right_tjunc = 3
    // holding_block = 1; //debug
    // magnetic = 0; //debug

    if (holding_block){
      switch (magnetic){
        case 0:
          //green box
          if (right_tjunctions_crossed == 1){
            // stop_motors(); //debug
            enter_square_on_right();

            left_servo.write(90);
            right_servo.write(90);

            delay(2000);

            //go backward
            set_motor_speed('L','B',255);
            set_motor_speed('R','B',255);
            delay(5000);

            //forward left turn
            set_motor_speed('L','F',255*0.3);
            set_motor_speed('R','F',255);
            delay(4000);

            //forward
            set_motor_speed('L','F',255);
            set_motor_speed('R','F',255);
            white_square_on_right = 1;
            follow_line();

            delay(99999999);

            
            turn_left();          
          }
          break;
        case 1:
        //red box
          if(right_tjunctions_crossed == 3){
            stop_motors();
            enter_square_on_right();

            left_servo.write(90);
            right_servo.write(90);            
            delay(2000);
            //go backward
            set_motor_speed('L','B',255);
            set_motor_speed('R','B',255);
            delay(7000);

            //forward right turn
            set_motor_speed('R','F',255*0.3);
            set_motor_speed('L','F',255);
            delay(2000);

            //forward
            set_motor_speed('L','F',255);
            set_motor_speed('R','F',255);
            white_square_on_left = 1;
            follow_line();
          }
      }
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

    turn_left();
    
    
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
    if (left_dist >=0 && left_dist <7){
      Serial.print("    slight rightward correction");
      set_motor_speed('L','F',default_speed);
      set_motor_speed('R','F',default_speed*0.8);    
    }
    else if (left_dist <= 15 && left_dist>7){
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



void turn_left(){
  current_state = "turning left";

  read_sensors();
  set_motor_speed('R','F',255);
  set_motor_speed('L','F',default_speed*0.3);

  if (left_white || right_white){
    Serial.println("found line");
    set_motor_speed('R','F',default_speed*0.8);
    set_motor_speed('L','F',default_speed);
    delay(500);

    follow_line();
    return;
  }

  delay(loop_delay);
  turn_left();
}

void turn_right(){
  current_state = "turning right";

  read_sensors();
  set_motor_speed('L','F',255);
  set_motor_speed('R','F',default_speed*0.3);

  if (left_white || right_white){
    Serial.println("found line");
    set_motor_speed('L','F',default_speed*0.8);
    set_motor_speed('R','F',default_speed);
    delay(500);

    follow_line();
    return;
  }
  
  delay(loop_delay);
  turn_right();
}

void enter_square_on_right(){
  current_state = "entering sqaure on right";
  
  //turn right
  set_motor_speed('L','F',255);
  set_motor_speed('R','F',default_speed*0);
  delay(4500);

  //go forward
  set_motor_speed('L','F',255);
  set_motor_speed('R','F',255);
  delay(1000);

  stop_motors();
}

void enter_square_on_left(){
  current_state = "entering sqaure on left";
  
  //turn right
  set_motor_speed('R','F',255);
  set_motor_speed('L','F',default_speed*0);
  delay(4500);

  //go forward
  set_motor_speed('R','F',255);
  set_motor_speed('L','F',255);
  delay(1000);

  stop_motors();
}




