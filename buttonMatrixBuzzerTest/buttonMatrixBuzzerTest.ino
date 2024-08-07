#define BUZZER_PIN 10

// Row pins
const int rowPins[3] = {1, 2, 3};

// Column pins
const int colPins[3] = {20, 21, 0};

// Note frequencies for each button
const int notes[3][3] = {
  {262, 294, 330}, // C4, D4, E4
  {349, 392, 440}, // F4, G4, A4
  {494, 523, 587}  // B4, C5, D5
};

void setup() {
  // Initialize row pins as outputs
  for (int i = 0; i < 3; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW); // Set all rows to LOW initially
  }

  // Initialize column pins as inputs with pull-ups
  for (int i = 0; i < 3; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }

  // Initialize the buzzer pin as output
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off initially
}

void loop() {
  for (int row = 0; row < 3; row++) {
    // Set the current row HIGH
    digitalWrite(rowPins[row], HIGH);
    
    for (int col = 0; col < 3; col++) {
      // Check if button in current column is pressed
      if (digitalRead(colPins[col]) == LOW) {
        // Play the corresponding note
        tone(BUZZER_PIN, notes[row][col]);
        
        // Wait for the button to be released
        while (digitalRead(colPins[col]) == LOW);
        
        // Stop the tone
        noTone(BUZZER_PIN);
      }
    }

    // Set the current row LOW again
    digitalWrite(rowPins[row], LOW);
  }
}
