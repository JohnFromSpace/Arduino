#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <AESLib.h> // Library for AES encryption
#include <Wire.h>
#include <IRremote.h>
#include <SparkFunBME280.h>
#include <Adafruit_Sensor.h>

SparkFunBME280 bme; // Create BME280 object
Adafruit_Sensor *humidity, *temp, *pressure, *altitude;

AESLib aes;

const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL}; // Addresses for communication pipes

RF24 radio(9, 10); // CE, CSN pins
const int buttonPin = 2; // Pin for the button
const int ledTransmit = 3; // LED indicating transmission
const int ledReceive = 4; // LED indicating reception

bool buttonState = false;

// Message buffer
const int BUFFER_SIZE = 10;
char messageBuffer[BUFFER_SIZE][64]; // Increase buffer size to accommodate larger messages
int bufferIndex = 0;

// Packet sequence number
uint16_t sequenceNumber = 0;

// Encryption key (16 bytes)
uint8_t aesKey[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00};

// Maximum number of retries
const int MAX_RETRIES = 3;

// Power-saving mode
const unsigned long SLEEP_INTERVAL = 60000; // Sleep for 1 minute
const int RTC_INTERRUPT_PIN = 5; // Pin for RTC interrupt

// Dynamic payload size adjustment
const int MAX_PAYLOAD_SIZE = 32; // Maximum payload size in bytes
int currentPayloadSize = MAX_PAYLOAD_SIZE; // Initial payload size

// Message fragmentation constants
const int MAX_FRAGMENT_SIZE = 28; // Maximum fragment size in bytes (to fit within 32-byte payload)
const char FRAGMENT_DELIMITER = '|'; // Delimiter for fragmented messages

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_250KBPS);
    radio.setRetries(15, 15);
    radio.setCRCLength(RF24_CRC_16); // Set CRC length
    radio.enableDynamicPayloads(); // Enable dynamic payload length
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
    radio.startListening();
    
    // Enable automatic channel hopping
    radio.setAutoChannel(true);
    
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledTransmit, OUTPUT);
    pinMode(ledReceive, OUTPUT);
    
         
}
