bool SensorOne;
bool SensorTwo;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  SensorOne = digitalRead(0);
  if (SensorOne == HIGH) {
    Serial.println("sensor 1 BLACK");
  }
  else{
    Serial.println("sensor 1 WHITE");
  }
  
  SensorTwo = digitalRead(1);

  bool left_black = SensorOne == HIGH;
  bool right_black = SensorTwo == HIGH;

  if (left_black && right_black){
    Serial.println("continue forward");
  }

  else if (left_black && !right_black){
    Serial.println("turn right");
  }

  else if (!left_black && right_black){
    Serial.println("turn left")
  }




  


  delay(100);

}
