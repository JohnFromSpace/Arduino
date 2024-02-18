// Include necessary libraries
#include <Servo.h>
#include <Keypad.h>
#include <EEPROM.h> 

// Define pins for the servo motors
#define SHOULDER_PIN 9  
#define ELBOW_PIN 10
#define WRIST_PIN 11

Servo shoulderServo;   // Create servo objects
Servo elbowServo;
Servo wristServo;

// Define neutral positions for the servos
const int shoulderNeutral = 90;  
const int elbowNeutral = 120;
const int wristNeutral = 90;

// Define pins for keypad matrix
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {A0, A1, A2, A3}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A5, 2, 3}; // Connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
