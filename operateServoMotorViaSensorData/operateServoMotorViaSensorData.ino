#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(4, 5); // RX, TX

Servo servo1, servo2, servo3, servo4; // Pins: 3, 9, 5, 6
Servo servos[4] = {servo1, servo2, servo3, servo4};

// Angles for each servo
int currentAngles[4] = {90, 90, 90, 90};  // Start at neutral

String incomingData = "";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  servos[0].attach(3); // s1, s6
  servos[1].attach(9); // s0, s5
  servos[2].attach(5); // count20, count21
  servos[3].attach(6); // count40, count41

  // Set initial angles
  for (int i = 0; i < 4; i++) {
    servos[i].write(currentAngles[i]);
  }

  Serial.println("Ready to receive data...");
}

void loop() {
  while (mySerial.available()) {
    char c = mySerial.read();
    
    if (c == '\n') {
      incomingData.trim();  // Remove any trailing \r or spaces
      Serial.print("Received: ");
      Serial.println(incomingData);

      int servoIndex = -1;
      int angleChange = 0;

      // Mapping logic
      if (incomingData == "s1" || incomingData == "s6") {
        servoIndex = 0;
        angleChange = (incomingData == "s1") ? -90 : 90;
      }
      else if (incomingData == "s0" || incomingData == "s5") {
        servoIndex = 1;
        angleChange = (incomingData == "s0") ? -90 : 90;
      }
      else if (incomingData == "count20" || incomingData == "count21") {
        servoIndex = 2;
        angleChange = (incomingData == "count20") ? -90 : 90;
      }
      else if (incomingData == "count40" || incomingData == "count41") {
        servoIndex = 3;
        angleChange = (incomingData == "count40") ? -90 : 90;
      }

      // Apply movement if command was valid
      if (servoIndex != -1) {
        currentAngles[servoIndex] = constrain(currentAngles[servoIndex] + angleChange, 0, 180);
        servos[servoIndex].write(currentAngles[servoIndex]);
        Serial.print("Servo ");
        Serial.print(servoIndex);
        Serial.print(" moved to ");
        Serial.println(currentAngles[servoIndex]);
      }

      incomingData = ""; // Clear buffer for next command
    }
    else {
      incomingData += c;
    }
  }
}
