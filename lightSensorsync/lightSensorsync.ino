// === Pin Configuration for 74HC165 (Sensor Input) ===
const int load = 7;             // PL (Parallel Load)
const int clockEnablePin = 4;   // CE (Clock Enable)
const int dataIn = 5;           // Q7 (Serial Output)
const int clockIn = 6;          // CP (Clock)

// === Pin Configuration for 74HC595 (LED Output) ===
const int dataPin = 8;    // DS
const int latchPin = 9;   // ST_CP
const int clockPin = 10;  // SH_CP

const int numShiftRegisters = 5; // 5 shift registers = 40 outputs
byte ledState[numShiftRegisters] = {0}; // Stores LED states for 5 registers

// === LED Matrix: 9 Light Posts, 3 LEDs each (Green, Yellow, Red) ===
// Indexes are 1-based in mapping, converted to 0-based internally
int matrix[9][3] = {
  {1, 2, 3},     // Post 1: Green, Yellow, Red
  {4, 5, 6},     // Post 2
  {9, 10, 11},   // Post 3
  {12, 13, 14},  // Post 4
  {17, 18, 19},  // Post 5
  {20, 21, 22},  // Post 6
  {25, 26, 27},  // Post 7
  {28, 29, 30},  // Post 8
  {33, 34, 35}   // Post 9
};

int dataSensor[7];
int countSens2=0;
int countSens4=0;
// === Function to Set a Specific LED ON/OFF ===
void setLED(int ledIndex, bool state) {
  ledIndex -= 1; // Convert 1-based to 0-based
  if (ledIndex < 0 || ledIndex >= numShiftRegisters * 8) return;

  int byteIndex = ledIndex / 8;
  int bitIndex = ledIndex % 8;

  if (state)
    ledState[byteIndex] |= (1 << bitIndex);
  else
    ledState[byteIndex] &= ~(1 << bitIndex);
}

// === Send LED States to Shift Registers ===
void updateLEDs() {
  digitalWrite(latchPin, LOW);
  for (int i = numShiftRegisters - 1; i >= 0; i--) {
    shiftOut(dataPin, clockPin, MSBFIRST, ledState[i]);
  }
  digitalWrite(latchPin, HIGH);
}

// === Read Sensor Data from 74HC165 ===
byte readSensors() {
  digitalWrite(load, LOW);
  delayMicroseconds(5);
  digitalWrite(load, HIGH);
  delayMicroseconds(5);

  digitalWrite(clockEnablePin, LOW);
  byte sensorData = shiftIn(dataIn, clockIn, LSBFIRST);
  digitalWrite(clockEnablePin, HIGH);

  return sensorData;
}

// === Setup ===
void setup() {
  Serial.begin(9600);

  // Sensor shift register pins
  pinMode(load, OUTPUT);
  pinMode(clockEnablePin, OUTPUT);
  pinMode(clockIn, OUTPUT);
  pinMode(dataIn, INPUT);

  // LED shift register pins
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  // Initialize all light posts to RED
  for (int i = 0; i < 9; i++) {
    setLED(matrix[i][0], false);  // Green OFF
    setLED(matrix[i][1], false);  // Yellow OFF
    setLED(matrix[i][2], true);   // Red ON
  }

  updateLEDs();
}

// === Main Loop ===
void loop() {
  byte sensors = readSensors();

  for (int i = 0; i <= 6; i++) {
    dataSensor[i] = bitRead(sensors, 6 - i);
  }

  bool anySensorActive = false;
   int temp =7;
   int green=-1,yellow=-1;
  // Check if any sensor is HIGH
  for (int i = 0; i <= 6; i++) {
    if (dataSensor[i] == 1 && temp!= i) {
      anySensorActive = true;
      temp=i;
      if(i==2){
        countSens2++;
        countSens2=countSens2%4;
      }
      if(i==4){
         countSens4++;
         countSens4=countSens4%2;
      }
      break;
    }
  }

  if (anySensorActive) {
    // No sensors active → reset to RED
  // Initialize all light posts to RED
  for (int i = 0; i < 9; i++) {
    setLED(matrix[i][0], false);  // Green OFF
    setLED(matrix[i][1], false);  // Yellow OFF
    setLED(matrix[i][2], true);   // Red ON
  }
      if(green!=-1 && yellow !=-1){
        setLED(matrix[green][0],false);
        setLED(matrix[yellow][1],false);
      }
    if (dataSensor[0] == 1 && dataSensor[1] == 0) {
      setLED(matrix[0][0], true);
      setLED(matrix[0][2], false);
      green=0;
      yellow=1;
      setLED(matrix[1][1], true);
      setLED(matrix[1][2], false);
    } else if (dataSensor[1] == 1 && dataSensor[2] == 0) {
      setLED(matrix[1][0], true);
      setLED(matrix[1][2], false);
      green=1;
      yellow=2;
      setLED(matrix[2][1], true);
      setLED(matrix[2][2], false);
    } else if (dataSensor[2] == 1) {
  //    countSens2++;
      if (countSens2 <=1 ) {
        if (dataSensor[3] == 0) {
          setLED(matrix[2][0], true);
          setLED(matrix[2][2], false);
          green=2;
          yellow=3;
          setLED(matrix[3][1], true);
          setLED(matrix[3][2], false);
        }
      } else { //if (countSens2 > 1) {
        if (dataSensor[4] == 0) {
          setLED(matrix[4][0], true);
          setLED(matrix[4][2], false);
           green=4;
           yellow=5;
          setLED(matrix[5][1], true);
          setLED(matrix[5][2], false);
        }
      }
    } else if (dataSensor[3] == 1 && dataSensor[1] == 0) {
      setLED(matrix[3][0], true);
      setLED(matrix[3][2], false);
     green=3;
      yellow=0;
      setLED(matrix[0][1], true);
      setLED(matrix[0][2], false);
    } else if (dataSensor[4] == 1) {
      if (countSens4 == 1) {
        if (dataSensor[5] == 0) {
          setLED(matrix[5][0], true);
          setLED(matrix[5][2], false);
         green=5;
         yellow=6;
          setLED(matrix[6][1], true);
          setLED(matrix[6][2], false);
          countSens4++;
        }
      } else {
        if (dataSensor[6] == 0) {
          setLED(matrix[7][0], true);
          setLED(matrix[7][2], false);
          green=7;
          yellow=8;
          setLED(matrix[8][1], true);
          setLED(matrix[8][2], false);
        }
      }
    } else if (dataSensor[5] == 1 && dataSensor[1] == 0) {
      setLED(matrix[6][0], true);
      setLED(matrix[6][2], false);
      green=6;
      yellow=1;
      setLED(matrix[1][1], true);
      setLED(matrix[1][2], false);
    }
  }
  updateLEDs();
  delay(1000);
}