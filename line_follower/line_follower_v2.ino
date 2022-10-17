#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' - currently left = M1 and right = M2
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(2);

// Declaring variables

int default_speed = 192;

bool left_sensor;
bool right_sensor;

int left_motor_speed = 0;
int right_motor_speed = 0;


void setup() {
  // set up Serial library at 9600 bps
  Serial.begin(9600);          
  Serial.println("Line follower test");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");


  // start each motor running at default speed
  left_motor->setSpeed(default_speed);
  left_motor->run(FORWARD);
  left_motor->run(RELEASE);

  right_motor->setSpeed(default_speed);
  right_motor->run(FORWARD);
  right_motor->run(RELEASE);

}

void loop() {
  
  left_sensor = digitalRead(0);
  right_sensor = digitalRead(1);

  if (left_sensor == HIGH){
    Serial.println("left sensor BLACK");
  }
  else{
    Serial.println("left sensor WHITE")
  }

  if (right_sensor == HIGH){
    Serial.println("right sensor BLACK");
  }
  else{
    Serial.println("right sensor WHITE")
  }

  if (left_sensor && right_sensor){
    Serial.println("continue forward");
    right_motor->setSpeed(default_speed);
    left_motor->setSpeed(default_speed);
  }

  else if (left_sensor && !right_sensor){
    Serial.println("turn right");
    right_motor->setSpeed(default_speed*0.9);
    left_motor->setSpeed(default_speed);
  }

  else if (!left_sensor && right_sensor){
    Serial.println("turn left");
    right_motor->setSpeed(default_speed);
    left_motor->setSpeed(default_speed*0.9);
  }

  delay(50);

}