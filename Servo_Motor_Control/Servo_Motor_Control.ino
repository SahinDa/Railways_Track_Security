#include <Servo.h>

Servo myServo; // Create a Servo object

void setup() {
  myServo.attach(9);  // Attach the servo to pin 9
  myServo.write(0);   // Initialize the servo at 0°
  Serial.begin(9600); // Start Serial communication
  Serial.println("Enter 1 to move to 90°, Servo will return to 0° after 2 seconds.");
}

void loop() {
  // Check if there is input from Serial Monitor
  if (Serial.available() > 0) {
    char input = Serial.read(); // Read the input

    if (input == '1') { // If input is 1
      myServo.write(85); // Move servo to 90°
      Serial.println("Servo moved to 90°");
      delay(2000); // Wait for 2 seconds
      myServo.write(0); // Move servo back to 0°
      Serial.println("Servo moved to 0°");
    } else {
      Serial.println("Invalid input! Enter 1 only.");
    }
  }
}
