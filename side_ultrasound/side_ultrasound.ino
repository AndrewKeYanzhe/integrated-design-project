/* Example code for HC-SR04 ultrasonic distance sensor with Arduino. No library required. More info: https://www.makerguides.com */

// Define Trig and Echo pin:
#define trigPin 5
#define echoPin 4

// Define variables:
long duration;
int side_distance;
int prev_side_dist = -1;

void setup() {
  // Define inputs and outputs:
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
  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance:
  side_distance = duration * 0.034 / 2;

  if (prev_side_dist == -1 && side_distance <=15){ //max side distance in tunnel is about 10
    prev_side_dist = side_distance;
  }


  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
  Serial.println("");
  Serial.print("Distance = ");
  Serial.print(side_distance);
  Serial.print(" ");
  Serial.print("    Prev dist =");
  Serial.print(prev_side_dist);

  // prev_side_dist = side_distance;



  delay(50);
}