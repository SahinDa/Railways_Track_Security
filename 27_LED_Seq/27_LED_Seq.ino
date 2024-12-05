int latchPin = 5;  // Latch pin of 74HC595
int clockPin = 6;  // Clock pin of 74HC595
int dataPin = 4;   // Data pin of 74HC595

// Define the matrix of LEDs (15x9) for 27 LEDs
int matrix[15][9] = { 
   {1, 5, 9, 12, 15, 18, 21, 24, 27},
   {4, 8, 12, 15, 18, 21, 24, 27, 3},
   {7, 11, 15, 18, 21, 24, 27, 3, 6},
   {10, 2, 6, 9, 15, 18, 21, 24, 27},
   {1, 5, 9, 12, 15, 18, 21, 24, 27},
   {4, 14, 3, 9, 12, 18, 21, 24, 27},
   {13, 17, 21, 24, 27, 3, 6, 9, 12},
   {16, 20, 24, 27, 3, 6, 9, 12, 15},
   {19, 5, 24, 27, 3, 9, 12, 15, 18},
   {4, 14, 18, 21, 24, 27, 3, 9, 12},
   {13, 23, 27, 3, 6, 9, 12, 18, 21},
   {22, 26, 3, 6, 9, 12, 15, 18, 21},
   {25, 8, 3, 6, 12, 15, 18, 21, 24},
   {7, 11, 15, 18, 21, 24, 27, 3, 6},
   {10, 2, 6, 9, 15, 18, 21, 24, 27}
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  // Loop through each group in the matrix
  for (int i = 0; i < 15; i++) {
    uint32_t ledPattern = 0;  // Initialize 32-bit LED pattern to 0

    // Set the bits corresponding to the current group in the matrix
    for (int j = 0; j < 9; j++) {
      ledPattern |= (1UL << (matrix[i][j] - 1));  // Turn on LEDs for this group
    }

    // Send the pattern to the shift registers
    digitalWrite(latchPin, LOW);  // Prepare to send data

    // Send the first 8 bits (high byte 1)
    shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 16) & 0xFF);

    // Send the middle 8 bits (high byte 2)
    shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 8) & 0xFF);

    // Send the last 8 bits (low byte)
    shiftOut(dataPin, clockPin, MSBFIRST, ledPattern & 0xFF);

    digitalWrite(latchPin, HIGH);  // Latch the data to the LEDs

    delay(5000);  // Wait for 5 seconds before moving to the next group
  }
}
