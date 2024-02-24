// Include necessary libraries
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <SD.h>

// Define pin numbers
const int blowerPin = 9;                // Blower control pin
const int potentiometerPin = A0;        // Potentiometer pin
const int buttonPin = 2;                // Push button pin
const int temperatureSensorPin = A1;    // Temperature sensor pin
const int chipSelectPin = 10;           // SD card CS pin

// Bluetooth module configuration
SoftwareSerial bluetoothSerial(8, 7);   // RX, TX pins of Bluetooth module
char receivedChar;

// LCD configuration
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // Initialize the LCD

// PID constants
double Kp = 0.1;  // Proportional constant
double Ki = 0.05; // Integral constant
double Kd = 0.02; // Derivative constant

// PID variables
double setpoint = 0; // Target blower speed
double output = 0;   // PID output
double input = 0;    // Actual blower speed
double integral = 0; // Integral term
double lastInput = 0; // Previous actual speed
double derivative = 0; // Derivative term
unsigned long lastTime = 0;
double dt = 100; // Time interval for PID calculation (in milliseconds)

// Blower operating modes
enum OperatingMode {
  MANUAL,
  AUTOMATIC
};
OperatingMode mode = MANUAL; // Initial operating mode

bool blowerState = false; // Blower state
unsigned long overheatingStartTime = 0; // Overheating start time
const unsigned long overheatingTimeout = 300000; // Overheating timeout (5 minutes)

// SD card logging
File dataFile;

void performCalibration() {
  // Display calibration message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrating...");
  
  // Perform calibration steps here
  // Measure system response to step input or other calibration method
  // Adjust PID constants accordingly
  
  // Example: Set PID constants to default values initially
  Kp = 0.1;
  Ki = 0.05;
  Kd = 0.02;

  // Display calibrated PID constants
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration done");
  lcd.setCursor(0, 1);
  lcd.print("Kp: ");
  lcd.print(Kp);
  lcd.setCursor(8, 1);
  lcd.print("Ki: ");
  lcd.print(Ki);
  lcd.setCursor(0, 2);
  lcd.print("Kd: ");
  lcd.print(Kd);
  
  // Delay to display calibration results (adjust as needed)
  delay(3000);
}

void setup() {
  // Set pin modes
  pinMode(blowerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  bluetoothSerial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Blower Speed:");

  // Initialize the SD card
  if (!SD.begin(chipSelectPin)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Error opening datalog.txt!");
    return;
  }
  dataFile.println("Time(ms),Blower Speed, Temperature(C)");

  // Perform automatic calibration
  performCalibration();
}

void loop() {
  // Read potentiometer value and map it to PWM range
  int potValue = analogRead(potentiometerPin);
  setpoint = map(potValue, 0, 1023, 0, 255);

  // Read button state
  bool buttonState = digitalRead(buttonPin);

  // Read temperature value
  int temperatureValue = analogRead(temperatureSensorPin);
  float temperatureCelsius = (temperatureValue / 1023.0) * 500;

  // Check for overheating
  if (temperatureCelsius > 70) {
    if (overheatingStartTime == 0) {
      overheatingStartTime = millis();
    } else {
      if (millis() - overheatingStartTime > overheatingTimeout) {
        blowerState = false;
        overheatingStartTime = 0;
      }
    }
  } else {
    overheatingStartTime = 0;
  }

  // Check for incoming Bluetooth data
  if (bluetoothSerial.available() > 0) {
    receivedChar = bluetoothSerial.read();
    if (receivedChar == '1') {
      blowerState = true;
      mode = AUTOMATIC; // Switch to automatic mode when blower is turned on remotely
    } else if (receivedChar == '0') {
      blowerState = false;
    } else if (receivedChar == 'K') {
      // Receive PID constants from Bluetooth
      if (bluetoothSerial.available() >= 3) {
        char kpStr[6], kiStr[6], kdStr[6];
        bluetoothSerial.readBytesUntil(',', kpStr, sizeof(kpStr));
        bluetoothSerial.readBytesUntil(',', kiStr, sizeof(kiStr));
        bluetoothSerial.readBytesUntil('\n', kdStr, sizeof(kdStr));
        Kp = atof(kpStr);
        Ki = atof(kiStr);
        Kd = atof(kdStr);
      }
    }
  }

  // Toggle blower state if button is pressed (in manual mode)
  if (mode == MANUAL && buttonState == LOW) {
    blowerState = !blowerState;
    delay(200); // Debounce delay
  }

  // Perform PID control if blower is on (in automatic mode)
  if (mode == AUTOMATIC || blowerState) {
    unsigned long now = millis();
    unsigned long timeChange = (now - lastTime);
    if (timeChange >= dt) {
      input = analogRead(potentiometerPin);
      double error = setpoint - input;
      integral += (error * timeChange);
      derivative = (input - lastInput) / timeChange;

      output = Kp * error + Ki * integral + Kd * derivative;

      if (output < 0) {
        output = 0;
      } else if (output > 255) {
        output = 255;
      }

      // Smoothly adjust blower speed
      if (abs(output - analogRead(blowerPin)) > 5) {
        analogWrite(blowerPin, output);
      }

      lastInput = input;
      lastTime = now;
    }
  } else {
    // Smoothly turn off blower if it's on
    if (analogRead(blowerPin) > 0) {
      analogWrite(blowerPin, 0);
    }
  }

  
}
