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
    switch (type) {
        case TimeOut:
            Serial.println("DFPlayer Timeout");
            break;
        case WrongStack:
            Serial.println("DFPlayer Wrong Stack");
            break;
        case DFPlayerCardInserted:
            Serial.println("DFPlayer Card Inserted");
            break;
        case DFPlayerCardRemoved:
            Serial.println("DFPlayer Card Removed");
            break;
        case DFPlayerCardOnline:
            Serial.println("DFPlayer Card Online");
            break;
        case DFPlayerPlayFinished:
            handlePlayFinished();
            break;
        case DFPlayerError:
            handleDFPlayerError(value);
            break;
        default:
            // Handle other responses
            Serial.print("DFPlayer Unknown Response Type: ");
            Serial.println(type);
            break;
    }    
}

void handlePlayFinished() {
    Serial.println("Play Finished");
    // Add your logic here for what to do when a track finishes playing
    // For example, you could play the next track in the playlist
    nextTrack();
}

void handleDFPlayerError(int errorCode) {
    switch (errorCode) {
        case Busy:
            Serial.println("DFPlayer Busy");
            break;
        case Sleeping:
            Serial.println("DFPlayer Sleeping");
            break;
        case SerialWrongStackCMD:
            Serial.println("DFPlayer Serial Wrong Stack CMD");
            break;
        case CheckSumNotMatch:
            Serial.println("DFPlayer Check Sum Not Match");
            break;
        case FileIndexOut:
            Serial.println("DFPlayer File Index Out of Bound");
            break;
        case FileMismatch:
            Serial.println("DFPlayer File Mismatch");
            break;
        case Advertise:
            Serial.println("DFPlayer Advertisement");
            break;
        default:
            Serial.print("DFPlayer Error: ");
            Serial.println(errorCode);
            break;
    }    
}

void displayError(const char* errorMessage) {
    lcd.clear();
    lcd.print(errorMessage);    
}

void playPause() {
    if (!isPlaying) {
        playTrack(currentTrack);
    } else {
        togglePause();
    }    
}

void nextTrack() {
    if (currentTrack < totalTracks) {
        currentTrack++;
        playTrack(currentTrack);
    }    
}

void prevTrack() {
    if (currentTrack > 1) {
        currentTrack--;
        playTrack(currentTrack);
    }    
}

void increaseVolume() {
    if (volume < 30) {
        volume++;
        myDFPlayer.volume(volume);
        updateVolumeDisplay();
    }    
}

void decreaseVolume() {
    if (volume > 0) {
        volume--;
        myDFPlayer.volume(volume);
        updateVolumeDisplay();
    }    
}
