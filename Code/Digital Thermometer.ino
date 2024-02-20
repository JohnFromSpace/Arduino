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
