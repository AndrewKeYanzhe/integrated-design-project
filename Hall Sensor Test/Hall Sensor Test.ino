const int hallEffectPin = 13;

void setup() {
  Serial.begin(9600);  

}

void loop() {
  // read the potentiometer:
  int hall_sensor = !digitalRead(hallEffectPin);
  // print the sensor reading
  Serial.println(hall_sensor);
  delay(100);
}