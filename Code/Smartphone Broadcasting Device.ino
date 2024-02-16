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

void loop() {
  if (bluetooth.available()) {
    String receivedString = bluetooth.readStringUntil('\n');
    if (receivedString.length() > 0) { // Check if the received string is not empty
      Serial.println(receivedString); // For debugging purposes

      // Clear the display buffer before writing new data
      display.clearDisplay();

      // Print the received string on the OLED display with automatic text wrapping
      display.setTextSize(1);
      display.setTextColor(WHITE);

      // Calculate the number of lines needed to display the text
      int numLines = (receivedString.length() * 6) / DISPLAY_WIDTH + 1;

      // Display each line of text
      for (int i = 0; i < numLines; i++) {
        String line = receivedString.substring(i * (DISPLAY_WIDTH / 6), (i + 1) * (DISPLAY_WIDTH / 6));
        display.setCursor(0, i * 8);
        display.println(line);
      }

      // Implement scrolling for long messages
      if (numLines > DISPLAY_HEIGHT / 8) {
        for (int i = 0; i < (numLines - DISPLAY_HEIGHT / 8) * 8; i++) {
          display.scroll(0, -1);
          delay(100);
        }
      }

      // Update the display
      display.display();
    }
  }
}
