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
  // Initialize Serial for debugging
  Serial.begin(115200);
  delay(1000); // Wait for serial to initialize
  
  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (true); // Loop forever if display initialization fails
  }
  display.clearDisplay();
  display.display();
  Serial.println("Display initialized");

  // Initialize the SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Card Mount Failed");
    while (true); // Loop forever if SD initialization fails
  }
  Serial.println("SD card initialized");

  // Display the filenames for 5 seconds
  displayFilenames("/bitmaps");

  // Main loop to display bitmaps
  while (true) {
    displayBitmapsFromFolder("/bitmaps");
    Serial.println("Finished one cycle");
  }
}

void displayFilenames(const char* folder) {
  File dir = SD.open(folder);
  if (!dir) {
    Serial.println("Failed to open directory");
    return; // Could not open directory
  }

  int y = 0; // Y-coordinate for displaying text
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  while (true) {
    File file = dir.openNextFile();
    if (!file) {
      break; // No more files
    }

    String fileName = file.name();
    if (!file.isDirectory() && fileName.endsWith(".bin")) {
      display.setCursor(0, y);
      display.print(fileName);
      y += 10; // Move to the next line (assuming 10 pixels per line)
      if (y > SCREEN_HEIGHT - 10) {
        y = 0;
        display.display();
        delay(2000); // Display batch of filenames for 2 seconds
        display.clearDisplay();
      }
    }
    file.close();
  }
  dir.close();
  display.display();
  delay(5000); // Display the last batch of filenames for 5 seconds
}

void displayBitmapsFromFolder(const char* folder) {
  File dir = SD.open(folder);
  if (!dir) {
    Serial.println("Failed to open directory");
    return; // Could not open directory
  }

  File file;
  while (file = dir.openNextFile()) {
    if (file && !file.isDirectory()) {
      String fileName = file.name();
      if (fileName.endsWith(".bin")) {
        display.clearDisplay();
        displayBitmapFromSD(file);
        display.display();
        //delay(2000); // Display each image for 2 seconds
      }
    }
    file.close();
  }
  dir.close();
}

void displayBitmapFromSD(File file) {
  int index = 0;
  int byte;
  while (file.available()) {
    byte = file.read();
    for (int bit = 0; bit < 8; bit++) {
      display.drawPixel(index % SCREEN_WIDTH, index / SCREEN_WIDTH, (byte & (1 << (7 - bit))) ? SSD1306_BLACK : SSD1306_WHITE);
      index++;
      if (index >= SCREEN_WIDTH * SCREEN_HEIGHT) {
        Serial.println("Filled the screen");
        return; // Stop if we have filled the screen
      }
    }
  }
}

void loop() {
  // Do nothing in the loop
}
