#include <SoftwareSerial.h>

SoftwareSerial bluetooth(19, 18); // RX, TX

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  // Enable the Bluetooth module
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);
}

void loop() {
  if (bluetooth.available()) {
    char c = bluetooth.read();
    Serial.write(c);
  }

  if (Serial.available()) {
    char c = Serial.read();
    char x = "A";
    bluetooth.write(x);
    Serial.write(c);
  }
}