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
  
  // Set up RTC interrupt
  pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
  
  // Initialize BME280 sensor
  bme.begin();
  humidity = bme.getHumiditySensor();
  temp = bme.getTemperatureSensor();
  pressure = bme.getPressureSensor();
  altitude = bme.getAltitudeSensor();         
}

void loop() {
  // Check button state
  bool newButtonState = digitalRead(buttonPin);
  if (newButtonState != buttonState) {
    buttonState = newButtonState;
    if (buttonState) {
      sendMessage("Button pressed! This is a high-priority message.", true);
    }
  }

    // Receive message
  if (radio.available()) {
    uint8_t len;
    if (radio.read(&len, sizeof(len))) {
      char receivedText[len + 1];
      if (radio.read(receivedText, len)) {
        receivedText[len] = '\0'; // Null-terminate the string
        
        // Decrypt message
        char decryptedText[len + 1];
        aes.do_aes_decrypt((uint8_t*)receivedText, len, (uint8_t*)decryptedText, aesKey, 128);
        
        // Store in message buffer
        if (bufferIndex < BUFFER_SIZE) {
          strcpy(messageBuffer[bufferIndex], decryptedText);
          bufferIndex++;
        }
        else {
          Serial.println("Message buffer full. Incoming message discarded.");
        }
        
        Serial.println("Received: " + String(decryptedText));
        digitalWrite(ledReceive, HIGH); // Turn on receive LED
        
        // Check message integrity
        if (verifyIntegrity(decryptedText)) {
          // Simulate error detection
          if (random(0, 10) < 2) { // 20% chance of error
            Serial.println("Error detected in received message. Requesting retransmission...");
            sendMessage("Error detected, please retransmit.", false);
          } else {
            // Send acknowledgment
            sendMessage("ACK", false);
          }
        } else {
          Serial.println("Message integrity check failed. Ignoring message.");
        }
        
        delay(1000);
        digitalWrite(ledReceive, LOW); // Turn off receive LED
      }
    }
  }

  delay(100);

  // Transmit message from buffer
  if (bufferIndex > 0) {
    sendMessage(messageBuffer[0], false);
    // Shift message queue
    for (int i = 0; i < bufferIndex - 1; i++) {
      strcpy(messageBuffer[i], messageBuffer[i + 1]);
    }
    bufferIndex--;
  }

  // Enter power-saving mode
  delay(100); // Delay to ensure button press detection
  attachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN), goToSleep, LOW);
  delay(100); // Allow time for interrupt to trigger
  detachInterrupt(digitalPinToInterrupt(RTC_INTERRUPT_PIN));  
}

void sendMessage(const char *message, bool highPriority) {
  // Increment sequence number
  sequenceNumber++;

  // Encrypt message
  char encryptedText[100];
  aes.do_aes_encrypt((uint8_t*)message, strlen(message), (uint8_t*)encryptedText, aesKey, 128);

  // Compress data
  // Compression algorithm can be implemented here
  
  // Fragment message if necessary
  int messageLength = strlen(encryptedText);
  if (messageLength > currentPayloadSize) {
    int numFragments = (messageLength + MAX_FRAGMENT_SIZE - 1) / MAX_FRAGMENT_SIZE;
    for (int i = 0; i < numFragments; i++) {
      int fragmentSize = min(MAX_FRAGMENT_SIZE, messageLength - i * MAX_FRAGMENT_SIZE);
      char fragment[fragmentSize + 1];
      strncpy(fragment, encryptedText + i * MAX_FRAGMENT_SIZE, fragmentSize);
      fragment[fragmentSize] = '\0'; // Null-terminate the fragment
      sendMessageFragment(fragment, numFragments, i, highPriority);
    }
  } else {
    // Send message as a single fragment
    sendMessageFragment(encryptedText, 1, 0, highPriority);
  }
}

void sendMessageFragment(const char *fragment, int numFragments, int fragmentIndex, bool highPriority) {
  // Construct fragment header
  String fragmentHeader = String(numFragments) + FRAGMENT_DELIMITER + String(fragmentIndex) + FRAGMENT_DELIMITER;

  // Construct complete message
  String completeMessage = fragmentHeader + fragment;

  
}
