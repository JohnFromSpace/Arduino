// Include necessary libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>

// Define OneWire and DallasTemperature instances
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

// Define LCD instance
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Wi-Fi credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// ThingSpeak settings
const char* server = "api.thingspeak.com";
const String apiKey = "YourAPIKey";

// Email settings
const char* smtpServer = "smtp.gmail.com";
const uint16_t smtpPort = 587;
const char* emailSender = "your.email@gmail.com";
const char* emailPassword = "YourEmailPassword";
const char* emailRecipient = "recipient@example.com";

// Thresholds for temperature alarm
float highAlarmThreshold = 30.0; // Set initial high temperature threshold (in Celsius)
float lowAlarmThreshold = 15.0;  // Set initial low temperature threshold (in Celsius)

// Sampling rate variables
const unsigned long samplingInterval = 10000; // Sampling interval in milliseconds
unsigned long previousMillis = 0; // Stores the time at which the last reading was taken

// Variables for data analysis
float sumTemperature = 0;
float minTemperature = 1000; // Initialize minimum temperature to a high value
float maxTemperature = -1000; // Initialize maximum temperature to a low value
int numReadings = 0; // Number of temperature readings
const int maxReadings = 100; // Maximum number of readings to store for analysis
