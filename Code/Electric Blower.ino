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
