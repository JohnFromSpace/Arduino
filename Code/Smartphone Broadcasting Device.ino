#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

SoftwareSerial bluetooth(10, 11); // RX, TX pins for Bluetooth module

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Clear the display buffer
  display.clearDisplay();
  display.display();
}
