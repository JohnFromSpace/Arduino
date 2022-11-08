int ledPin = 9; //pin for the LED
int photoCellInput = 0; //pin for the photoresistor

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  photoCellInput = (analogRead(0)/4); //conversion from 0-1023 to  0-255
  analogWrite(ledPin, photoCellInput);
  delay(20); //delaying it in order to achieve a dimming effect
}
