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
    Serial.println("BLACK");
  }
  else{
    Serial.println("WHITE");
  }
  
  //SensorTwo = digitalRead(1);
  //if (SensorTwo == HIGH){
  //  Serial.println("Sensor two HIGH");
  //}
  //else{
  //  Serial.println("Sensor two LOW");
  //}

  delay(100);

}
