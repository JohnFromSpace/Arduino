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
