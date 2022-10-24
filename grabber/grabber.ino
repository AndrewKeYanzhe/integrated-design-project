// Grabber
// servo.write is clockwise

#include <Servo.h>
#include <Adafruit_MotorShield.h>

#define trigPin 2
#define echoPin 3

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Servo left_servo;
Servo right_servo;

//for ultrasound
long duration;
int ultrasound_dist;

//variables to change
int contact_distance = 10;

void setup() {
  //for ultrasound
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // set up Serial library at 9600 bps
  Serial.begin(9600);
  AFMS.begin();

//   Serial.println("");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Attach a servo to pin #10 - this is Servo 1 on the motor shield 
  left_servo.attach(10);
  right_servo.attach(11); //CHANGE

  

}

void loop() {

  if (distance < contact_distance){
    // lift up
  left_servo.write(-1);
  right_servo.write(1);
  }

  else if (distance > contact_distance){
    Serial.println("block picked up OR ultrasound_dist is far");
  }

  delay(1);

}

void read_ultrasound(){
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
  Serial.print(distance);
  Serial.println(" cm");

  return ultrasound_dist;  
}