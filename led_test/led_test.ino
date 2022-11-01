
int green_pin = 11; //analog
int red_pin = 12;
int amber_pin = 13;//amber is 12, digital

void setup() {
  Serial.begin(9600);  



  //LEDs
  pinMode(green_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(amber_pin,OUTPUT);

  // analogWrite(green_pin, 0);
  // analogWrite(red_pin, 0);
  // analogWrite(amber_pin,0);

  // delay(9999999999);

  digitalWrite(green_pin, 1);
  digitalWrite(red_pin, 1);
  digitalWrite(amber_pin,1);

}

void loop() {
  

  delay(1);
}