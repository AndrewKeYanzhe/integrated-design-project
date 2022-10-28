/* Example code for HC-SR04 ultrasonic distance sensor with Arduino. No library required. More info: https://www.makerguides.com */

// Define Trig and Echo pin:
#define trigPin 5
#define echoPin 4

// Define variables for ultrasound:
long ultrasound_duration;
int left_distance;


void setup() {
  // Define inputs and outputs for ultrasound:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Begin Serial communication at a baudrate of 9600:
  Serial.begin(9600);


}

void loop() {
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(15);

  // Trigger the sensor by setting th1234e trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  // Read the echoPin, pulseIn() returns the ultrasound_duration (length of the pulse) in microseconds:
  ultrasound_duration = pulseIn(echoPin, HIGH);
  // Calculate the distance:
  left_distance = ultrasound_duration * 0.034 / 2;

  // if (prev_side_dist == -1 && left_distance <=15){ //max side distance in tunnel is about 10. Ideal distance is 6
  //   prev_side_dist = left_distance;
  // }


  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
  Serial.println("");
  Serial.print("Distance = ");
  Serial.print(left_distance);
  Serial.print(" ");
  // Serial.print("    Prev dist =");
  // Serial.print(prev_side_dist);




  delay(50);
}