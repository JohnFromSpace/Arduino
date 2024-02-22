#include <LiquidCrystal.h>

// Pin definitions
const int espressoButtonPin = 2;
const int latteButtonPin = 3;
const int cappuccinoButtonPin = 4;
const int pumpPin = 5;
const int waterLevelPin = A0;
const int waterTempPin = A1;
const int waterFilterPin = A2;
const int grinderPin = 6;
const int buzzerPin = 7;
const int strengthButtonPin = 8;
const int timerButtonPin = 9;

// Variables to store button state
int espressoButtonState = 0;
int latteButtonState = 0;
int cappuccinoButtonState = 0;
int strengthButtonState = 0;
int timerButtonState = 0;

// LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Brewing parameters
const int espressoBrewTime = 3000; // 3 seconds
const int latteBrewTime = 5000;    // 5 seconds
const int cappuccinoBrewTime = 7000; // 7 seconds

// Coffee types
enum CoffeeType {
    ESPRESSO,
    LATTE,
    CAPPUCCINO
};

// Strength levels
enum StrengthLevel {
    REGULAR,
    STRONG
};

// Function to brew coffee
void brewCoffee(CoffeeType type, int brewTime, StrengthLevel strength) {
    // Check water level
    int waterLevel = analogRead(waterLevelPin);
    if (waterLevel < 100) {
        lcd.clear();
        lcd.print("Water Low!");
        tone(buzzerPin, 1000, 1000); // Beep to indicate water low
        delay(2000);
        return;
    }

    
}
