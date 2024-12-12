// Pin configuration for 74HC595 shift register
int latchPin = 5;  // Latch pin of 74HC595
int clockPin = 6;  // Clock pin of 74HC595
int dataPin = 4;   // Data pin of 74HC595

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  // Light up all 27 LEDs
  uint32_t ledPattern = 0x07FFFFFF;  // 27 bits set to 1 (binary: 111...111)

  digitalWrite(latchPin, LOW);  // Prepare to send data

  // Send the highest byte (bits 16-23)
  shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 16) & 0xFF);

  // Send the middle byte (bits 8-15)
  shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 8) & 0xFF);

  // Send the lowest byte (bits 0-7)
  shiftOut(dataPin, clockPin, MSBFIRST, ledPattern & 0xFF);

  digitalWrite(latchPin, HIGH);  // Latch the data to the LEDs
}

void loop() {
  // Nothing to do here, LEDs will stay on
}
