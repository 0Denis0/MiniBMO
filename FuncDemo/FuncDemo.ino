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
void listFiles(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // No more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      listFiles(entry, numTabs + 1);
    } else {
      // Files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial to initialize
  }

  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Initialize the SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  
  // List files on SD card
  File root = SD.open("/");
  listFiles(root, 0);
  root.close();
  
  // Wait 3 seconds
  delay(3000);

  // Initialize the IMU
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
}

void loop() {
  // Display moving text
  static int x = 0;
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, 10);
  display.print(F("Sample Text"));
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
