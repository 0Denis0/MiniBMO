#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>

// SD Card setup
#define SD_CS_PIN 7

// Display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// IMU setup
MPU6050 mpu;

// Function to list files on SD card
void listFilesToDisplay(File dir) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  int line = 0;
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // No more files
      break;
    }
    if (line >= 8) { // Maximum lines to display on 128x64 screen with text size 1
      display.display();
      delay(2500); // Display current files for 2.5 seconds before showing next set
      display.clearDisplay();
      display.setCursor(0, 0);
      line = 0;
    }

    display.print(entry.name());
    display.print(entry.isDirectory() ? "/" : "");
    display.println();
    line++;

    entry.close();
  }
  display.display();
}

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

  // List files on SD card and display them
  File root = SD.open("/");
  listFilesToDisplay(root);
  root.close();

  // Wait 5 seconds
  delay(5000);

  // Initialize the IMU
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (true); // Loop forever if IMU connection fails
  }
}

void loop() {
  // Display moving text
  static int x = 0;
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, 10);
  display.print(F("Text 123"));
  x = (x + 1) % SCREEN_WIDTH;

  // Get IMU readout
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Display IMU values
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("AX: "); display.print(ax);
  display.setCursor(0, 40);
  display.print("AY: "); display.print(ay);
  display.setCursor(0, 50);
  display.print("AZ: "); display.print(az);

  display.setCursor(64, 30);
  display.print("GX: "); display.print(gx);
  display.setCursor(64, 40);
  display.print("GY: "); display.print(gy);
  display.setCursor(64, 50);
  display.print("GZ: "); display.print(gz);

  display.display();
  delay(100);
}
