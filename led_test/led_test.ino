
int green_pin = 0; //analog
int red_pin = 1;
int amber_pin = 2;//amber is 12, digital

void setup() {
  Serial.begin(9600);  



  //LEDs
  pinMode(green_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(amber_pin,OUTPUT);

  analogWrite(green_pin, 0);
  analogWrite(red_pin, 0);
  analogWrite(amber_pin,0);

  delay(9999999999);

  analogWrite(green_pin, 255);
  analogWrite(red_pin, 255);
  analogWrite(amber_pin,255);

}

void loop() {
  

  delay(1);
}