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
