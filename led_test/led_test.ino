
int green_pin = 5; //analog
int red_pin = 9;
int amber_pin = 12;//amber is 12, digital

void setup() {
  Serial.begin(9600);  



  //LEDs
  pinMode(green_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(amber_pin,OUTPUT);

  analogWrite(green_pin, 255);
  analogWrite(red_pin, 255);
  digitalWrite(amber_pin,HIGH);

}

void loop() {
  

  delay(1);
}