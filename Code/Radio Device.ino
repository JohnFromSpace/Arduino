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
