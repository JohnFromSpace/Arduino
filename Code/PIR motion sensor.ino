int ledPin = 13;
int pirPin = 2;

int pirValue;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  
  digitalWrite(ledPin, LOW);
  delay(9600);
}

void loop() {
  pirValue = digitalRead(pirPin);
  digitalWrite(ledPin, pirValue);
  Serial.print(pirValue);
  Serial.println();
  delay(500);
}
