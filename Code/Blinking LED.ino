int ledPin = 13; //number of pin for LED
int inPin = 2; //number of pin for button 
int val = 0; //counting states of the pin

void setup() {
  pinMode(ledPin, OUTTPUT); //declaring the pin as an output 
  pinMode(inPin, INPUT); //declaring the pin as input
}

void loop() {
  val = digitalRead(inPin); //reads the value of the input pin
  if(val == HIGH) {         //checks whether the pin for input is pressed
    digitalWrite(ledPin, LOW); //turns off the pin
  }
  else {
    digitalWrite(ledPin, HIGH); //turns on the pin
  }
}
