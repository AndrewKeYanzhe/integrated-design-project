// Grabber
// servo.write is clockwise

#include <Servo.h>
//#include <Adafruit_MotorShield.h>

#define trigPin 6
#define echoPin 5

// Create the motor shield object with the default I2C address
//Adafruit_MotorShield AFMS = Adafruit_MotorShield();

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
  //AFMS.begin();

//   Serial.println("");

//  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
  //  Serial.println("Could not find Motor Shield. Check wiring.");
    //while (1);
  //}
  //Serial.println("Motor Shield found.");

  // Attach a servo to pin #10 - this is Servo 1 on the motor shield 
  left_servo.attach(10);
  right_servo.attach(9); //CHANGE

  left_servo.write(90);
  right_servo.write(90);
  Serial.println("1");
  // delay(999999);
  // delay(3000);
  Serial.println("2");
  // delay(3000);

  int angle = 90;



  for (int i=0;i<=angle;i++){
    left_servo.write(90+i);
    right_servo.write(90-i);
    delay(100);
  }

  delay(2000);
  left_servo.write(90);
  right_servo.write(90);

  

}

void loop() {

  ultrasound_dist = read_ultrasound();

  ultrasound_dist = 999; // to change

  if (ultrasound_dist < contact_distance){
    // lift up
  left_servo.write(-1);
  right_servo.write(1);
  }

  else if (ultrasound_dist > contact_distance){
    Serial.println("block picked up OR ultrasound_dist is far");
  }

  delay(1);

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
