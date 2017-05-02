#include <servo.h>

void setup() {
  servoInit();
}

void loop() {
  servoSetPosition(1000);
  delay(1500);
  servoSetPosition(2000);
  delay(1500);
}
