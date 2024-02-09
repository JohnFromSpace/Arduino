// Define pin numbers
const int motorPin = 9;          // Pin connected to the motor
const int directionPin = 10;     // Pin connected to direction control
const int potentiometerPin = A0; // Pin connected to potentiometer

// Define motor directions
const int clockwise = HIGH;
const int counterclockwise = LOW;

// Define minimum and maximum speed values
const int minSpeed = 50;  // Minimum speed required to start the motor
const int maxSpeed = 255;

// Define ramp up/down duration (in milliseconds)
const unsigned int rampDuration = 1000; // 1 second

// Define timeout for stop command (in milliseconds)
const unsigned long stopTimeout = 5000; // 5 seconds

unsigned long lastStopTime = 0; // Variable to store the time when stop command was last received
int currentSpeed = 0; // Variable to store the current speed of the motor
bool motorRunning = false; // Flag to indicate if the motor is running
bool directionChanging = false; // Flag to indicate if the direction is being changed

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set motor and direction pins as outputs
  pinMode(motorPin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  
  // Set initial direction (clockwise)
  digitalWrite(directionPin, clockwise);
}

void loop() {
  // Read speed from potentiometer
  int targetSpeed = map(analogRead(potentiometerPin), 0, 1023, minSpeed, maxSpeed);
  
  // Smoothly adjust motor speed
  smoothSpeed(targetSpeed);
  
  // Check for stop command timeout
  if (millis() - lastStopTime >= stopTimeout && motorRunning) {
    stopMotor();
  }
  
  // Check if data is available in the serial buffer
  if (Serial.available() > 0) {
    // Read the command from serial input
    char command = Serial.read();
    
    
  }
  
  
}
