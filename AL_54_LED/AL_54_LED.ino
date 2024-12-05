int latchPin = 5;  // Latch pin of 74HC595
int clockPin = 6;  // Clock pin of 74HC595
int dataPin = 4;   // Data pin of 74HC595

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  turnOn54LEDs();
}

void loop() {
  // Keep the LEDs on continuously
}

// Function to turn on all 54 LEDs
void turnOn54LEDs() {
  digitalWrite(latchPin, LOW);  // Prepare to send data

  // Send data for 7 shift registers
  shiftOut(dataPin, clockPin, MSBFIRST, 0xFF); // First 8 LEDs on
  shiftOut(dataPin, clockPin, MSBFIRST, 0xFF); // Next 8 LEDs on
  shiftOut(dataPin, clockPin, MSBFIRST, 0xFF); // Next 8 LEDs on
  shiftOut(dataPin, clockPin, MSBFIRST, 0xFF); // Next 8 LEDs on
  shiftOut(dataPin, clockPin, MSBFIRST, 0xFF); // Next 8 LEDs on
  shiftOut(dataPin, clockPin, MSBFIRST, 0xFF); // Next 8 LEDs on
  shiftOut(dataPin, clockPin, MSBFIRST, 0x3F); // Last 6 LEDs on (00111111 in binary)

  digitalWrite(latchPin, HIGH);  // Latch the data to the LEDs
}
