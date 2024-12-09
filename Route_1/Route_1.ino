// Pins for 74HC595
int latchPin = 5;  // Latch pin of 74HC595
int clockPin = 6;  // Clock pin of 74HC595
int dataPin = 4;   // Data pin of 74HC595

// Pins for 74HC165
int load = 7;  // PL pin (pin 1)
int clockEnablePin = 4;  // CE pin (pin 15)
int dataIn = 5;  // Q7 pin (pin 7)
int clockIn = 6;  // CP pin (pin 2)

// LED Matrix (15x9) - Assuming this is your matrix layout
int matrix[15][9] = { 
   {1, 5, 9, 12, 15, 18, 21, 24, 27},   // Index 0 (Sensor 1)
   {4, 8, 12, 15, 18, 21, 24, 27, 3},   // Index 1 (Sensor 2)
   {7, 11, 15, 18, 21, 24, 27, 3, 6},   // Index 2 (Sensor 3)
   {10, 2, 6, 9, 15, 18, 21, 24, 27},  // Index 3 (Sensor 4)
   {1, 5, 9, 12, 15, 18, 21, 24, 27},   // Index 4 (Sensor 1)
   {4, 14, 3, 9, 12, 18, 21, 24, 27},  // Index 5 (Sensor 2)
   {13, 17, 21, 24, 27, 3, 6, 9, 12},  // Index 6 (Sensor 3)
   {16, 20, 24, 27, 3, 6, 9, 12, 15},  // Index 7 (Sensor 5)
   {19, 5, 24, 27, 3, 9, 12, 15, 18},  // Index 8 (Sensor 6)
   {4, 14, 18, 21, 24, 27, 3, 9, 12},  // Index 9 (Sensor 2)
   {13, 23, 27, 3, 6, 9, 12, 18, 21}, // Index 10 (Sensor 3)
   {22, 26, 3, 6, 9, 12, 15, 18, 21}, // Index 11 (Sensor 5)
   {25, 8, 3, 6, 12, 15, 18, 21, 24}, // Index 12 (Sensor 7)
   {7, 11, 15, 18, 21, 24, 27, 3, 6}, // Index 13 (Sensor 3)
   {10, 2, 6, 9, 15, 18, 21, 24, 27}  // Index 14 (Sensor 4)
};

// Sensor-to-index mapping based on your explained logic
int sensorToMatrixIndex[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}; 

// Current active sensor
int currentSensor = -1;  // No sensor active at the start

// Cycle counter variables
int counter1 = 0;  // For deciding between Sensor 4 or Sensor 5
int counter2 = 0;  // For deciding between Sensor 6 or Sensor 7

void setup() {
  // Setup LED pins
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  // Setup sensor pins
  pinMode(load, OUTPUT);
  pinMode(clockEnablePin, OUTPUT);
  pinMode(clockIn, OUTPUT);
  pinMode(dataIn, INPUT);

  Serial.begin(9600);  // Initialize Serial monitor
}

void loop() {
  // Read sensor data
  digitalWrite(load, LOW);
  delayMicroseconds(5);
  digitalWrite(load, HIGH);
  delayMicroseconds(5);

  digitalWrite(clockIn, HIGH);
  digitalWrite(clockEnablePin, LOW);
  byte incoming = shiftIn(dataIn, clockIn, LSBFIRST);
  digitalWrite(clockEnablePin, HIGH);

  // Determine active sensor
  int activeSensor = -1;
  for (int i = 0; i < 8; i++) {
    if ((incoming >> i) & 1) {
      activeSensor = i + 1;  // Sensors are 1-indexed
      break;
    }
  }

  // Update the LED sequence only if a new sensor is detected
  if (activeSensor != -1 && activeSensor != currentSensor) {
    currentSensor = activeSensor;  // Update the current sensor
    
    // Handle Sensor 3 decision for counter1
    if (currentSensor == 3) {
      if (counter1 == 0 || counter1 == 1) {
        // Move to Sensor 4
        lightUpMatrix(3);  // Sensor 4
      } else {
        // Move to Sensor 5
        lightUpMatrix(7);  // Sensor 5
      }
      counter1++;  // Increment counter1 for next cycle
      counter1 %= 4;  // Keep counter1 modulo 4 (0-3)
    }

    // Handle Sensor 5 decision for counter2
    if (currentSensor == 5) {
      if (counter2 == 0) {
        // Move to Sensor 7
        lightUpMatrix(12);  // Sensor 7
      } else {
        // Move to Sensor 6
        lightUpMatrix(8);  // Sensor 6
      }
      counter2++;  // Increment counter2 for next cycle
      counter2 %= 2;  // Keep counter2 modulo 2 (0-1)
    }

    // Handle other sensor logic (e.g., regular sensor actions)
    else {
      int ledIndex = sensorToMatrixIndex[currentSensor - 1];  // Map sensor to matrix index
      lightUpMatrix(ledIndex);  // Update the LED sequence
    }
  }

  delay(500);  // Small delay for stability
}

// Function to light up the matrix for a given index
void lightUpMatrix(int index) {
  uint32_t ledPattern = 0;

  for (int j = 0; j < 9; j++) {
    ledPattern |= (1UL << (matrix[index][j] - 1));
  }

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 16) & 0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, (ledPattern >> 8) & 0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, ledPattern & 0xFF);
  digitalWrite(latchPin, HIGH);

  Serial.print("Lighting up matrix index: ");
  Serial.println(index);
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
