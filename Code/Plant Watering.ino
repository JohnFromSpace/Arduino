// Define the pin for the soil moisture sensor
const int soilMoisturePin = A0;

// Define the pin for the water pump
const int waterPumpPin = 8;

// Define the moisture threshold below which the system will water the plant
const int moistureThreshold = 500;

void setup() {
    // Initialize Serial communication
    Serial.begin(9600);
    
    // Set water pump pin as an output
    pinMode(waterPumpPin, OUTPUT);    
}

void loop() {
    // Read the soil moisture level
    int moistureLevel = analogRead(soilMoisturePin);
    
    // Print the moisture level to the Serial Monitor
    Serial.print("Moisture Level: ");
    Serial.println(moistureLevel);
    
    // Check if the soil moisture is below the threshold
    if (moistureLevel < moistureThreshold) {
        // Water the plant by turning on the water pump
        digitalWrite(waterPumpPin, HIGH);
        Serial.println("Watering the plant");
            
        // Add a delay to water for a certain duration (adjust as needed)
        delay(5000);  // Watering for 5 seconds (adjust as needed)
        
        // Turn off the water pump
        digitalWrite(waterPumpPin, LOW);
        Serial.println("Watering complete");        
    }
    
    // Add a delay before the next iteration (adjust as needed)
    delay(60000);  // Wait for 1 minute (adjust as needed)     
}
