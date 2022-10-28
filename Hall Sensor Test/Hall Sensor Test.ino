const int hallEffectPin = 13;

void setup() {
  Serial.begin(9600);  

}

void loop() {
  // read the potentiometer:
  int sensorReading = !digitalRead(hallEffectPin);
  // print the sensor reading
  Serial.println(sensorReading);
  delay(100);
}