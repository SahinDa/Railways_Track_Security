int latchPin = 5;  // Latch pin of 74HC595
int clockPin = 6;  // Clock pin of 74HC595
int dataPin = 4;   // Data pin of 74HC595

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 27; i++) {  // Loop through all 27 LEDs
    uint32_t ledPattern = (1UL << i);  // Light up one LED at a time

    // Send the pattern to the shift registers
    digitalWrite(latchPin, LOW);  // Prepare to send data

    // Send all 4 bytes of the pattern (from most significant to least significant byte)
    shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 24) & 0xFF);  // First byte (most significant)
    shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 16) & 0xFF);  // Second byte
    shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 8) & 0xFF);   // Third byte
    shiftOut(dataPin, clockPin, MSBFIRST, ledPattern & 0xFF);          // Fourth byte (least significant)

    digitalWrite(latchPin, HIGH);  // Latch the data to the LEDs

    delay(500);  // Wait for 0.5 seconds before testing the next LED
  }
}
