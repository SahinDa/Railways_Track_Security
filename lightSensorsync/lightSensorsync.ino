#include <SoftwareSerial.h>


const int load = 7;            // PL (Parallel Load)Add commentMore actions
const int clockEnablePin = 4;  // CE (Clock Enable)
const int dataIn = 5;          // Q7 (Serial Output)
const int clockIn = 6;         // CP (Clock)

// === Pin Configuration for 74HC595 (LED Output) ===
const int dataPin = 8;    // DS
const int latchPin = 9;   // ST_CP
const int clockPin = 10;  // SH_CP
SoftwareSerial mySerial(3, 2);

const int numShiftRegisters = 6;           // 5 shift registers = 40 outputs
byte ledState[numShiftRegisters] = { 0 };  // Stores LED states for 5 registers

// === LED Matrix: 9 Light Posts, 3 LEDs each (Green, Yellow, Red) ===
// Indexes are 1-based in mapping, converted to 0-based internally
int matrix[9][3] = {
  { 1, 2, 3 },     // Post 1: Green, Yellow, Red
  { 4, 5, 6 },     // Post 2
  { 9, 10, 11 },   // Post 3
  { 12, 13, 14 },  // Post 4
  { 17, 18, 19 },  // Post 5
  { 20, 21, 22 },  // Post 6
  { 25, 26, 27 },  // Post 7
  { 28, 29, 30 },  // Post 8
  { 33, 34, 35 }   // Post 9
};
int reverse[]={36,37,38,41,42,43,44,45,46};
int path[] = { 0, 1, 2, 3, 0, 1, 2, 4, 5, 1, 2, 4, 6, 2, 3 };
int index = 0;

int dataSensor[7];
int countSens1 = 0;
int countSens2 = 0;
int countSens4 = 0;

bool anySensorActive = false;
int temp = 7;
int green = -1, yellow = -1;


// === Function to Set a Specific LED ON/OFF ===
void setLED(int ledIndex, bool state) {
  ledIndex -= 1;  // Convert 1-based to 0-based
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
  mySerial.begin(9600);
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

    //reverse
    setLED(reverse[i],true);
  }

  updateLEDs();
}

// === Main Loop ===
void loop() {
  byte sensors = readSensors();

  for (int i = 0; i <= 6; i++) {
    dataSensor[i] = 1 - bitRead(sensors, 6 - i);
    //   Serial.print(dataSensor[i]);
  }
  // Serial.println();


  // Check if any sensor is HIGH
  for (int i = 0; i <= 6; i++) {

    if (dataSensor[i] == 1 /* && temp!= i */ && i == path[index]) {

      anySensorActive = true;
      temp = path[index];
      // Serial.println(temp);
      /*      
      if(i==1 && temp!=index){
        countSens1++;
        countSens1=countSens1%3;
      }
      if(i==2 && temp!=index){
        countSens2++;
        countSens2=countSens2%4;
      }
      if(i==4 && temp!=index){
         countSens4++;
         countSens4=countSens4%3;
      }
      temp=index;
      break;
      */
    }
  }

  if (anySensorActive) {
    // No sensors active → reset to RED
    // Initialize all light posts to RED
    anySensorActive = false;
    for (int i = 0; i < 9; i++) {
      setLED(matrix[i][0], false);  // Green OFF
      setLED(matrix[i][1], false);  // Yellow OFF
      setLED(matrix[i][2], true);   // Red ON
    }
    if (green != -1 && yellow != -1) {
      setLED(matrix[green][0], false);
      setLED(matrix[yellow][1], false);
    }
    if (dataSensor[0] == 1 && dataSensor[1] == 0 && temp == 0) {
      setLED(matrix[0][0], true);
      setLED(matrix[0][2], false);
      green = 0;
      yellow = 1;

      index++;
      index %= 15;
      // mySerial.println("s0\n");
      setLED(matrix[1][1], true);
      setLED(matrix[1][2], false);
    }
    // else if(dataSensor[0] == 1 && dataSensor[1] == 1){
    // }
    else if (dataSensor[1] == 1 && dataSensor[2] == 0 && temp == 1) {

      setLED(matrix[1][0], true);
      setLED(matrix[1][2], false);
      // mySerial.println("s1\n");
      countSens1++;
      countSens1 = countSens1 % 3;
      index++;
      index %= 15;

      green = 1;
      if (countSens1 == 1) {
        yellow = 2;
        setLED(matrix[2][1], true);
        setLED(matrix[2][2], false);
      } else {
        yellow = 4;
        setLED(matrix[4][1], true);
        setLED(matrix[4][2], false);
      }
    }
    // else if(dataSensor[1] == 1 && dataSensor[2] == 1){
    // }
    else if (dataSensor[2] == 1 && temp == 2) {
      int count1 = (countSens2 + 1) % 4;
      //  Serial.println(temp);

      if (count1 <= 1) {
        if (dataSensor[3] == 0) {
          countSens2 = count1;
          // countSens2=countSens2%4;
         
          setLED(matrix[2][0], true);
          setLED(matrix[2][2], false);
                 index++;
          index %= 15;
          green = 2;
          yellow = 3;
          // mySerial.println("count20\n");
          setLED(matrix[3][1], true);
          setLED(matrix[3][2], false);
        }
      } else {  //if (countSens2 > 1) {

        if (dataSensor[4] == 0) {
          // Serial.println("true");
          setLED(matrix[4][0], true);
          setLED(matrix[4][2], false);
          countSens2 = count1;
          //  countSens2=countSens2%4;
          index++;
          index %= 15;
          green = 4;
          mySerial.println("count20\n");
          if (countSens2 == 2) {
            yellow = 6;
            setLED(matrix[5][1], true);
            setLED(matrix[5][2], false);
          } else {
            yellow = 7;
            setLED(matrix[7][1], true);
            setLED(matrix[7][2], false);
          }
        }
      }
    }
    //else if(dataSensor[2] == 1 && dataSensor[3]==1 || dataSensor[2] == 1 && dataSensor[4]==1){
    // }
    else if (dataSensor[3] == 1 && dataSensor[1] == 0 && temp == 3) {
      setLED(matrix[3][0], true);
      setLED(matrix[3][2], false);

      index++;
      index %= 15;

      green = 3;
      yellow = 0;
      setLED(matrix[0][1], true);
      setLED(matrix[0][2], false);
    }
    // else if(dataSensor[3] == 1 && dataSensor[1] == 1){
    // }
    else if (dataSensor[4] == 1 && temp == 4) {
      int count1 = (countSens4 + 1) % 3;
      if (count1 <= 1) {
        if (dataSensor[5] == 0) {
          setLED(matrix[5][0], true);
          setLED(matrix[5][2], false);
          countSens4 = count1;
          // countSens4=countSens4%3;
          index++;
          index %= 15;
          green = 5;
          yellow = 6;
          mySerial.println("count40\n");
          setLED(matrix[6][1], true);
          setLED(matrix[6][2], false);
          // countSens4++;
        }
      } else {
        if (dataSensor[6] == 0) {
          setLED(matrix[7][0], true);
          setLED(matrix[7][2], false);
          green = 7;
          yellow = 8;
          countSens4 = count1;
          // countSens4=countSens4%3;
          index++;
          index %= 15;
          // mySerial.println("s6\n");
          mySerial.println("count41\n");
          setLED(matrix[8][1], true);
          setLED(matrix[8][2], false);
        }
      }
    }
    // else if(dataSensor[4] == 1 && dataSensor[5]==1 || dataSensor[4] == 1 && dataSensor[6]==1){
    // }
    else if (dataSensor[5] == 1 && dataSensor[1] == 0 && temp == 5) {
      setLED(matrix[6][0], true);
      setLED(matrix[6][2], false);

      index++;
      index %= 15;
      green = 6;
      yellow = 1;
      mySerial.println("s5\n");
      setLED(matrix[1][1], true);
      setLED(matrix[1][2], false);
    }
    // else if(dataSensor[5] == 1 && dataSensor[1] == 1){
    // }
    else if (dataSensor[6] == 1 && dataSensor[2] == 0 && temp == 6) {
      setLED(matrix[8][0], true);
      setLED(matrix[8][2], false);

      index++;
      index %= 15;
      green = 8;
      yellow = 2;
      mySerial.println("s6\n");
      setLED(matrix[2][1], true);
      setLED(matrix[2][2], false);
    }
    Serial.println(index);
  }
  updateLEDs();
  delay(500);
}