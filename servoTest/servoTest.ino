#include <Servo.h>

Servo test;

int servoPin = 6;

void setup() {
  // put your setup code here, to run once:
  test.attach(servoPin,500,2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  test.write(90);
  delay(1500);
  test.write(0);
  delay(1500);
  test.write(90);
  delay(1500);
  test.write(180);
  delay(1500);
}
