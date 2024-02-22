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
