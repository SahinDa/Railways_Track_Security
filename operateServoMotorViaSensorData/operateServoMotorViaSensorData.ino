#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(4, 7); // RX, TX

Servo servo1, servo2, servo3, servo4; // Pins: 3, 8, 6, 5
Servo servos[4] = {servo1, servo2, servo3, servo4};

// Servo angle tracking
int currentAngles[4] = {0, 0, 0, 0};

// Time tracking for auto return
unsigned long lastMovedTime[4] = {0, 0, 0, 0};
const unsigned long returnDelay = 5000; // 5 seconds

String incomingData = "";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  servos[0].attach(3);  // s5
  servos[1].attach(8);  // s6
  servos[2].attach(6);  // count20
  servos[3].attach(5);  // count40

  // Set all servos to 0° at startup
  for (int i = 0; i < 4; i++) {
    currentAngles[i] = 0;
    servos[i].write(0);
  }

  Serial.println("Ready to receive data...");
}

void loop() {
  // Read serial data
  while (mySerial.available()) {
    char c = mySerial.read();

    if (c == '\n') {
      incomingData.trim();
      Serial.print("Received: ");
      Serial.println(incomingData);

      int servoIndex = -1;
      int angleChange = 0;

      // Command mapping
      if (incomingData == "s6") {
        servoIndex = 1;
        angleChange = +90;
      }
      else if (incomingData == "s5") {
        servoIndex = 0;
        angleChange = +90;
      }
      else if (incomingData == "count20") {
        servoIndex = 2;
        angleChange = +92;
      }
      else if (incomingData == "count40") {
        servoIndex = 3;
        angleChange = +90;
      }

      // Move servo if valid command
      if (servoIndex != -1) {
        currentAngles[servoIndex] = constrain(currentAngles[servoIndex] + angleChange, 0, 180);
        servos[servoIndex].write(currentAngles[servoIndex]);
        lastMovedTime[servoIndex] = millis();  // Track time
        Serial.print("Servo ");
        Serial.print(servoIndex);
        Serial.print(" moved to ");
        Serial.println(currentAngles[servoIndex]);
      }

      incomingData = ""; // Clear input
    } else {
      incomingData += c;
    }
  }

  // Auto return servos to 0 after delay
  for (int i = 0; i < 4; i++) {
    if (currentAngles[i] != 0 && (millis() - lastMovedTime[i]) >= returnDelay) {
      currentAngles[i] = 0;
      servos[i].write(0);
      Serial.print("Servo ");
      Serial.print(i);
      Serial.println(" returned to 0° after 5 sec");
    }
  }
}