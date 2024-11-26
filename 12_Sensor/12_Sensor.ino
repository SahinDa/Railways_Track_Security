// Define Arduino pins connected to the shift registers
int load = 7;          // PL pin 1 (parallel load)
int clockEnablePin = 4; // CE pin 15 (clock enable)
int dataIn = 5;         // Q7 pin 7(serial data out from second register)
int clockIn = 6;        // CP pin 2(clock pulse)

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Setup 74HC165 connections
  pinMode(load, OUTPUT);             // Parallel load pin
  pinMode(clockEnablePin, OUTPUT);   // Clock enable pin
  pinMode(clockIn, OUTPUT);          // Clock pulse pin
  pinMode(dataIn, INPUT);            // Serial data input pin
  
  // Set default states
  digitalWrite(clockIn, HIGH);       // Initialize clock to HIGH
  digitalWrite(clockEnablePin, HIGH); // Disable shift register (CE HIGH)
}

void loop() {
  // Load data from sensors into the shift registers
  digitalWrite(load, LOW);           // Pull PL LOW to load parallel data
  delayMicroseconds(5);              // Short delay
  digitalWrite(load, HIGH);          // Set PL HIGH for normal operation
  delayMicroseconds(5);              // Short delay

  // Enable reading from the shift registers
  digitalWrite(clockEnablePin, LOW); // Enable shift register output

  // Read 16 bits of data (covers both registers, though only 12 sensors are connected)
  unsigned int sensorData = 0;       // Variable to store combined data
  for (int i = 0; i < 16; i++) {
    sensorData |= (digitalRead(dataIn) << (15 - i)); // Read each bit
    digitalWrite(clockIn, LOW);      // Toggle clock LOW
    delayMicroseconds(5);            // Short delay
    digitalWrite(clockIn, HIGH);     // Toggle clock HIGH
  }

  // Disable reading from the shift registers
  digitalWrite(clockEnablePin, HIGH); // Disable shift register output

  // Print the sensor states
  Serial.print("Sensor States: ");
  for (int i = 10; i >= 0; i--) {    // Only print the first 12 bits (12 sensors)
    Serial.print((sensorData >> i) & 1); // Extract and print each bit
  }
  Serial.println();

  // Short delay before next read
  delay(200);
}
