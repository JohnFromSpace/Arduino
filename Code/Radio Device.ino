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
