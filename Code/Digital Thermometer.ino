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

// SD card chip select pin
const int chipSelect = 10;

void setup() {
    // Initialize Serial port
    Serial.begin(115200);
    
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    
    // Connect to Wi-Fi
    connectWiFi();
    
    // Start up the Dallas Temperature library
    sensors.begin();
    
    // Initialize SD card
    if (!SD.begin(chipSelect)) {
        Serial.println("SD card initialization failed...");
        return;
    }
    Serial.println("SD card initialized...");
    
    // Set ThingSpeak settings
    WiFiClient client;
    
    // Initialize LCD
    lcd.init();
    lcd.backlight();
}

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= samplingInterval) {
        previousMillis = currentMillis;
        
        // Read temperature
        sensors.requestTemperatures();
        float temperature = sensors.getTempCByIndex(0);
        
        // Update statistics
        updateStatistics(temperature);
        
        // Log temperature data to SD card
        logTemperatureData(temperature);
    
        // Check temperature against thresholds
        if (temperature > highAlarmThreshold || temperature < lowAlarmThreshold) {
            sendEmailAlert(temperature);
        }
    }
}

void connectWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void sendEmailAlert(float temperature) {
    WiFiClientSecure client;
    if (client.connect(smtpServer, smtpPort)) {
        client.println("HELO example.com");
        client.println("MAIL FROM:<" + String(emailSender) + ">");
        client.println("RCPT TO:<" + String(emailRecipient) + ">");
        client.println("DATA");
        client.println("Subject: Temperature Alert");
        client.println("From: <" + String(emailSender) + ">");
        client.println("To: <" + String(emailRecipient) + ">");
        client.print("Temperature has exceeded threshold: ");
        client.print(temperature);
        client.println("°C");
        client.println(".");
        client.println("QUIT");
    }
}

void updateStatistics(float temperature) {
    // Increment number of readings
    numReadings++;
    
    // Update sum of temperatures
    sumTemperature += temperature;
    
    // Update minimum temperature
    minTemperature = min(minTemperature, temperature);
    
    // Update maximum temperature
    maxTemperature = max(maxTemperature, temperature);
    
    // Update LCD display
    updateLCD(temperature);
}

void updateLCD(float temperature) {
    // Clear LCD
    lcd.clear();
        
    // Display current temperature
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");
    
    // Display statistics
    lcd.setCursor(0, 1);
    lcd.print("Min: ");
    lcd.print(minTemperature);
    lcd.print(" Max: ");
    lcd.print(maxTemperature);
    
    // Display average temperature
    lcd.setCursor(0, 2);
    lcd.print("Avg: ");
    lcd.print(sumTemperature / numReadings);
    
    
}
