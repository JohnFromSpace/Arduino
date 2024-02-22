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
    
    // Check water temperature
    int waterTemp = analogRead(waterTempPin);
    float temperature = (waterTemp / 1023.0) * 5000 / 10; // Convert to Celsius
    if (temperature < 85 || temperature > 95) {
        lcd.clear();
        lcd.print("Water Temp Err!");
        tone(buzzerPin, 1500, 1000); // Beep to indicate temperature error
        delay(2000);
        return;
    }
    
    // Check water filter
    int waterFilter = analogRead(waterFilterPin);
    if (waterFilter < 100) {
        lcd.clear();
        lcd.print("Replace Filter!");
        tone(buzzerPin, 2000, 1000); // Beep to indicate filter replacement needed
        delay(2000);
    }
    
    // Display selected coffee and strength
    lcd.clear();
    switch (type) {
        case ESPRESSO:
            lcd.print("Espresso");
            break;
        case LATTE:
            lcd.print("Latte");
            break;
        case CAPPUCCINO:
            lcd.print("Cappuccino");
            break;
    }
    lcd.setCursor(0, 1);
    lcd.print("Strength:");
    lcd.setCursor(10, 1);
    lcd.print(strength == REGULAR ? "Regular" : "Strong");
    lcd.setCursor(0, 2);
    lcd.print("Brewing...");

    // Start pump
    digitalWrite(pumpPin, HIGH);

    // Brew coffee
    digitalWrite(buzzerPin, HIGH); // Sound buzzer to indicate brewing started
    delay(brewTime);
    digitalWrite(buzzerPin, LOW); // Stop buzzer
    digitalWrite(pumpPin, LOW); // Stop pump

    // Display message
    lcd.clear();
    lcd.print("Enjoy your coffee!");
    delay(2000);
}

void setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize LCD
    lcd.begin(16, 2);

    // Initialize button pins
    pinMode(espressoButtonPin, INPUT);
    pinMode(latteButtonPin, INPUT);
    pinMode(cappuccinoButtonPin, INPUT);
    pinMode(strengthButtonPin, INPUT);
    pinMode(timerButtonPin, INPUT);

    // Initialize pump pin
    pinMode(pumpPin, OUTPUT);

    // Initialize buzzer pin
    pinMode(buzzerPin, OUTPUT);

    // Set initial state
    digitalWrite(pumpPin, LOW);
    digitalWrite(buzzerPin, LOW);
}

void loop() {
    // Read the state of the buttons
    espressoButtonState = digitalRead(espressoButtonPin);
    latteButtonState = digitalRead(latteButtonPin);
    cappuccinoButtonState = digitalRead(cappuccinoButtonPin);
    strengthButtonState = digitalRead(strengthButtonPin);
    timerButtonState = digitalRead(timerButtonPin);

    
}
