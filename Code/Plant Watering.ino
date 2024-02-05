// Define the pin for the soil moisture sensor
const int soilMoisturePin = A0;

// Define the pin for the water pump
const int waterPumpPin = 8;

// Define the moisture threshold below which the system will water the plant
const int moistureThreshold = 500;

void setup() {
    // Initialize Serial communication
    Serial.begin(9600);

    
}
