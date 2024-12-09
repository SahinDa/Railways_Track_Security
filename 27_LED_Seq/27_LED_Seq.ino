int latchPin = 5;  // Latch pin of 74HC595
int clockPin = 6;  // Clock pin of 74HC595
int dataPin = 4;   // Data pin of 74HC595

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Define your 32-bit binary pattern here
  //uint32_t ledPattern =0b1001001001001001001000100001;
 // Example: First bit ON, rest OFF
uint32_t ledPattern =0b111111111111111000000000000;
  // Debugging: Print the pattern to Serial Monitor
  Serial.print("Pattern: ");
  Serial.println(ledPattern, BIN);

  // Send the pattern to the shift registers
  digitalWrite(latchPin, LOW);  // Prepare to send data

  // Send all 4 bytes of the pattern
  shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 24) & 0xFF);  // First byte (most significant)
  shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 16) & 0xFF);  // Second byte
  shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 8) & 0xFF);   // Third byte
  shiftOut(dataPin, clockPin, MSBFIRST, ledPattern & 0xFF);          // Fourth byte (least significant)

  digitalWrite(latchPin, HIGH);  // Latch the data to the LEDs

  delay(2000);  // Keep the pattern for 2 seconds
}
