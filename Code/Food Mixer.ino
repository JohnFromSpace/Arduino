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