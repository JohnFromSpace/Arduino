// Include necessary libraries
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal.h>

// Pin Definitions
#define RX                10
#define TX                11
#define SD_CS             4
#define BUTTON_PLAY_PAUSE 2
#define BUTTON_NEXT       3
#define BUTTON_PREV       5
#define BUTTON_VOL_UP     6
#define BUTTON_VOL_DOWN   7
#define LCD_RS            8
#define LCD_EN            9
#define LCD_D4            12
#define LCD_D5            13
#define LCD_D6            A0
#define LCD_D7            A1
#define POTENTIOMETER_PIN A2

// Global Variables
SoftwareSerial mySerial(RX, TX);
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
bool isPlaying = false;
bool isPaused = false;
int volume = 20;
int currentTrack = 1;
int totalTracks = 0;
char currentTrackInfo[64] = "";

void setup() {
    // Serial Communication
    Serial.begin(9600);
    mySerial.begin(9600);
    
    // LCD Initialization
    lcd.begin(16, 2);
    lcd.print("MP3 Player");
    
    // Button Setup
    pinMode(BUTTON_PLAY_PAUSE, INPUT_PULLUP);
    pinMode(BUTTON_NEXT, INPUT_PULLUP);
    pinMode(BUTTON_PREV, INPUT_PULLUP);
    pinMode(BUTTON_VOL_UP, INPUT_PULLUP);
    pinMode(BUTTON_VOL_DOWN, INPUT_PULLUP);

    // SD Card Initialization
    if (!SD.begin(SD_CS)) {
        displayError("SD Card Error!");
        return;
    }
    
    // DFPlayer Initialization
    if (!myDFPlayer.begin(mySerial)) {
        displayError("DFPlayer Error!");
        return;
    }
    
    myDFPlayer.setTimeOut(500);
    myDFPlayer.volume(volume);
    totalTracks = myDFPlayer.readFileCounts();

    // Button Interrupts
    attachInterrupt(digitalPinToInterrupt(BUTTON_PLAY_PAUSE), playPause, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_NEXT), nextTrack, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PREV), prevTrack, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_VOL_UP), increaseVolume, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_VOL_DOWN), decreaseVolume, FALLING);   
}

void loop() {
    if (myDFPlayer.available()) {
        handleDFPlayerResponse(myDFPlayer.readType(), myDFPlayer.read());
    }
    
    int potValue = analogRead(POTENTIOMETER_PIN);
    int mappedValue = map(potValue, 0, 1023, 1, totalTracks);
    if (mappedValue != currentTrack) {
        currentTrack = mappedValue;
        playTrack(currentTrack);
    }

    delay(100);    
}

void handleDFPlayerResponse(uint8_t type, int value) {
    
}
