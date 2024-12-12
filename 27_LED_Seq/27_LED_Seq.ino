int latchPin = 5;  // Latch pin of 74HC595
int clockPin = 6;  // Clock pin of 74HC595
int dataPin = 4;   // Data pin of 74HC595

// Define the matrix of LEDs (3x9) representing light posts
// Each row represents a color (Green, Yellow, Red)
// Each column represents a light post
int matrix[15][9] = { 
   {1, 5, 11, 14, 19, 22, 27, 30, 35},  // Index 0 (Sensor 1)
   {4, 10, 14, 19, 22, 27, 30, 35, 3},  // Index 1 (Sensor 2)
   {9, 13, 19, 22, 27, 30, 35, 3, 6},   // Index 2 (Sensor 3)
   {12, 2, 6, 11, 19, 22, 27, 30, 35},  // Index 3 (Sensor 4)
   {1, 5, 11, 14, 19, 22, 27, 30, 35},  // Index 4 (Sensor 1)
   {4, 18, 3, 11, 14, 22, 27, 30, 35},  // Index 5 (Sensor 2)
   {17, 21, 27, 30, 35, 3, 6, 11, 14},  // Index 6 (Sensor 3)
   {20, 26, 30, 35, 3, 6, 11, 14, 19},  // Index 7 (Sensor 5)
   {25, 5, 30, 35, 3, 11, 14, 19, 22},  // Index 8 (Sensor 6)
   {4, 18, 22, 27, 30, 35, 3, 11, 14},  // Index 9 (Sensor 2)
   {17, 29, 35, 3, 6, 11, 14, 22, 27},  // Index 10 (Sensor 3)
   {28, 34, 3, 6, 11, 14, 19, 22, 27},  // Index 11 (Sensor 5)
   {33, 10, 3, 6, 14, 19, 22, 27, 30},  // Index 12 (Sensor 7)
   {9, 13, 19, 22, 27, 30, 35, 3, 6},   // Index 13 (Sensor 3)
   {12, 2, 6, 11, 19, 22, 27, 30, 35}   // Index 14 (Sensor 4)
};


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  // Loop through each color (Green, Yellow, Red)
  for (int i = 0; i < 15; i++) {
    uint64_t ledPattern = 0;  // Initialize LED pattern to 0 (64 bits for 5 shift registers)

    // Set the bits corresponding to the current group (color) in the matrix
    for (int j = 0; j < 9; j++) {
      ledPattern |= (1ULL << (matrix[i][j] - 1));  // Turn on LEDs for this color group
    }

    // Send the pattern to the shift registers
    digitalWrite(latchPin, LOW);  // Prepare to send data

    // Send the data (5 bytes for 40 bits)
    shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 32) & 0xFF);  // Send the most significant byte
    shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 24) & 0xFF);  // Send the next byte
    shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 16) & 0xFF);  // Send the next byte
    shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 8) & 0xFF);   // Send the next byte
    shiftOut(dataPin, clockPin, MSBFIRST, ledPattern & 0xFF);          // Send the least significant byte

    digitalWrite(latchPin, HIGH);  // Latch the data to the LEDs

    delay(5000);  // Wait for 5 seconds before moving to the next color group
  }
}