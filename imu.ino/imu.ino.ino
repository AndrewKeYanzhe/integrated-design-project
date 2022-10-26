#include <Arduino_LSM6DS3.h>

void setup() {
  // put your setup code here, to run once:
  IMU.begin()
}

void loop() {
  // put your main code here, to run repeatedly:
  float x, y, z;
  IMU.readAcceleration(x, y, z);
  Serial.print(x);
  Serial.print('\t');
  Serial.print(y);
  Serial.print('\t');
  Serial.println(z);

  \* Correct zero errors - we will need to find these *\
  float cor_x, cor_y, cor_z;
  cor_x = x + 1.0;
  cor_y = y + 1.0;
  cor_z = z + 1.0;

  //let z be the variable we care about for turns. Will need to test this and whether acceleration is positive in the clockwise or anticlockwise test, as well as testing for a zero error.
  float avg = 0;
  int pos_counter = 0;
  int neg_counter = 0;
  int number_of_left_turns;
  int number_of_right_turns;
  //Assumed that z is the acceleration we care about - will probably need to edit this equation
  avg = 0.7*avg + 0.3*cor_z;
  // Will need to edit these values
  if (avg > 5.0);
    ++pos_counter;
  if (avg < -5.0);
    ++neg_counter;
  if (pos_counter > 200);
    ++number_of_left_turns;
  if (neg_counter > 200); 
    ++number_of_right_turns;
  if (-2.5 < avg < 2.5);
    pos_counter = 0;
    neg_counter = 0;
  delay(10);
}
