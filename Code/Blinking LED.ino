int ledPin = 13; // declare the pin that the LED is connected to

void setup() {
  pinMode(ledPin, OUTPUT); // declare the pin as used for output
}

void loop() {
  digitalWrite(ledPin, HIGH); // turns on the LED
  delay(1000); // waits for 1 second
  digitalWrite(ledPin, LOW); // turns off the LED
  delay(1000); // waits for 1 second
} // the cycle repeats indefinitely and thus we achieve the blinking effect
