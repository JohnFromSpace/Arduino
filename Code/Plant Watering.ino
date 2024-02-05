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
        
            
    }
    
     
}
