#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SD Card setup
#define SD_CS_PIN 7

// Display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Loop forever if display initialization fails
  }
  display.clearDisplay();
  display.display();

  // Initialize the SD card
  if (!SD.begin(SD_CS_PIN)) {
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    return;
  }

  // Read bitmap from SD card and display it
  File bitmapFile = SD.open("/bitmap.bin");
  if (bitmapFile) {
    displayBitmapFromSD(bitmapFile);
    bitmapFile.close();
  }

  display.display();
}

void displayBitmapFromSD(File file) {
  int index = 0;
  int byte;
  while (file.available()) {
    byte = file.read();
    for (int bit = 0; bit < 8; bit++) {
      display.drawPixel(index % SCREEN_WIDTH, index / SCREEN_WIDTH, (byte & (1 << (7 - bit))) ? SSD1306_WHITE : SSD1306_BLACK);
      index++;
      if (index >= SCREEN_WIDTH * SCREEN_HEIGHT) {
        return; // Stop if we have filled the screen
      }
    }
  }
}

void loop() {
  // Do nothing in the loop
}
