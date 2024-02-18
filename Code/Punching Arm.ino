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

// Define structure to hold user settings
struct UserSettings {
  int force;
  int speed;
};

byte rowPins[ROWS] = {A0, A1, A2, A3}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A5, 2, 3}; // Connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Define pins for pneumatic system
#define SOLENOID_PIN 13
#define PRESSURE_SENSOR_PIN A6

// Define punching force thresholds
const int minForce = 0;
const int maxForce = 1023;

// Define feedback LED pin
#define FEEDBACK_LED_PIN 7

// Define EEPROM address for storing user settings
#define EEPROM_ADDRESS 0

// Function prototypes
void performPunch(int direction);
void moveArm(int shoulderAngle, int elbowAngle, int wristAngle, int duration);
void resetArm();
void adjustPneumaticPressure(int targetForce);
void saveSettings();
void loadSettings();
void gestureRecognition();
void saveSettings();
void loadSettings();

// Global variable to hold user settings
UserSettings currentUserSettings;

void setup() {
  shoulderServo.attach(SHOULDER_PIN);  // Attach servo objects to pins
  elbowServo.attach(ELBOW_PIN);
  wristServo.attach(WRIST_PIN);
  
  // Initialize servo positions
  shoulderServo.write(shoulderNeutral);
  elbowServo.write(elbowNeutral);
  wristServo.write(wristNeutral);

  pinMode(SOLENOID_PIN, OUTPUT); // Set solenoid pin as output
  pinMode(FEEDBACK_LED_PIN, OUTPUT); // Set feedback LED pin as output
  
  // Load user settings
  loadSettings();
  
  Serial.begin(9600);
}

void loop() {
  //gestureRecognition();  // Uncomment to enable gesture recognition
  
  char key = keypad.getKey(); // Read keypad input
  
  if (key != NO_KEY) {
    // Perform punching action based on keypad input
    int direction = key - '0'; // Convert char to integer
    performPunch(direction);
  }

  loadSettings();
}

void performPunch(int direction) {
  // Adjust servo angles based on punching direction
  int shoulderAngle = shoulderNeutral;
  int elbowAngle = elbowNeutral;
  int wristAngle = wristNeutral;
  
  switch(direction) {
    case 1:  // Adjust servo angles for different punching directions
      shoulderAngle += 30;
      break;
    case 2:
      shoulderAngle -= 30;
      break;
    case 3:
      elbowAngle += 30;
      break;
    case 4:
      elbowAngle -= 30;
      break;
    case 5:
      wristAngle += 30;
      break;
    case 6:
      wristAngle -= 30;
      break;
    default:
      return; // Exit if invalid direction
  }

  // Move arm to punching position
  moveArm(shoulderAngle, elbowAngle, wristAngle, 500);
  
  // Adjust pneumatic pressure for dynamic force adjustment
  adjustPneumaticPressure(userForce);
  
  // Display feedback
  displayFeedback();
  
  // Reset arm to neutral position after punch
  resetArm();
}

void moveArm(int shoulderAngle, int elbowAngle, int wristAngle, int duration) {
  // Move servos to specified angles over the given duration
  
  int shoulderCurrent = shoulderServo.read();
  int elbowCurrent = elbowServo.read();
  int wristCurrent = wristServo.read();
  
  int shoulderSteps = abs(shoulderAngle - shoulderCurrent);
  int elbowSteps = abs(elbowAngle - elbowCurrent);
  int wristSteps = abs(wristAngle - wristCurrent);
  
  int shoulderDirection = (shoulderAngle > shoulderCurrent) ? 1 : -1;
  int elbowDirection = (elbowAngle > elbowCurrent) ? 1 : -1;
  int wristDirection = (wristAngle > wristCurrent) ? 1 : -1;
  
  int shoulderDelay = duration / shoulderSteps;
  int elbowDelay = duration / elbowSteps;
  int wristDelay = duration / wristSteps;
  
  for (int i = 1; i <= shoulderSteps; i++) {
    shoulderServo.write(shoulderCurrent + i * shoulderDirection);
    delay(shoulderDelay);
  }
  for (int i = 1; i <= elbowSteps; i++) {
    elbowServo.write(elbowCurrent + i * elbowDirection);
    delay(elbowDelay);
  }
  for (int i = 1; i <= wristSteps; i++) {
    wristServo.write(wristCurrent + i * wristDirection);
    delay(wristDelay);
  }
}

void resetArm() {
  // Move arm back to neutral position
  moveArm(shoulderNeutral, elbowNeutral, wristNeutral, 500);
}

void increasePressure() {
  // Activate a solenoid valve or pump to increase pressure
  digitalWrite(SOLENOID_PIN, HIGH); // Example: activate a solenoid valve
  // Alternatively, control a pump to increase pressure
}

void decreasePressure() {
  // Deactivate a solenoid valve or release pressure
  digitalWrite(SOLENOID_PIN, LOW); // Example: deactivate a solenoid valve
  // Alternatively, release pressure from the system
}

void adjustPneumaticPressure(int targetForce) {
  // Read current force from the pressure sensor
  int currentForce = analogRead(PRESSURE_SENSOR_PIN);

  // Define threshold values for pressure adjustment
  int pressureThreshold = 50; // Adjust as needed

  if (currentForce < targetForce - pressureThreshold) {
    // Increase pressure to increase force
    increasePressure();
  } else if (currentForce > targetForce + pressureThreshold) {
    // Decrease pressure to reduce force
    decreasePressure();
  }
}
