int latchPin = 5;  // Latch pin of 74HC595
int clockPin = 6;  // Clock pin of 74HC595
int dataPin = 4;   // Data pin of 74HC595

/*int matrix[6][3] = {
  {1, 2, 3},  // Group 1 (First Light Post)
  {4, 5, 6},  // Group 2 (Second Light Post)
  {7, 8, 9},  // Group 3 (Third Light Post)
  {10, 11, 12}, // Group 4 (Fourth Light Post)
  {13, 14, 15}, // Group 5 (Fifth Light Post)
  {16, 16, 16}  // Group 6 (Sixth Light Post, LED 16 only)
}; */

// Define the matrix of LEDs (3x5) representing light posts
// Each row represents a color (Green, Yellow, Red)
// Each column represents a light post
int matrix[4][5] = {
   {1, 4, 8, 12, 15},  
   {3, 4, 7, 11, 15}, 
   {3, 6, 7, 10, 14},
   {2,6,9,10,13} 
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  // Loop through each color (Green, Yellow, Red)
  for (int i = 0; i < 3; i++) {
    uint16_t ledPattern = 0;  // Initialize LED pattern to 0

    // Set the bits corresponding to the current group (color) in the matrix
    for (int j = 0; j < 5; j++) {
      ledPattern |= (1 << (matrix[i][j] - 1));  // Turn on LEDs for this color group
    }

    // Send the pattern to the shift registers
    digitalWrite(latchPin, LOW);  // Prepare to send data

    // Send the high byte (first 8 bits)
    shiftOut(dataPin, clockPin, MSBFIRST, highByte(ledPattern));

    // Send the low byte (next 8 bits)
    shiftOut(dataPin, clockPin, MSBFIRST, lowByte(ledPattern));

    digitalWrite(latchPin, HIGH);  // Latch the data to the LEDs

    delay(5000);  // Wait for 5 seconds before moving to the next color group
  }
}