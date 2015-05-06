#include <Servo.h>

Servo test;

int center = 101;
int servoPin = 9;
int servoAdd = 45;
int servoSubtract = 30;

void setup() {
  // put your setup code here, to run once:
  test.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
  test.write(center);
  delay(1500);
 // test.write(center+servoAdd);
  delay(1500);
  test.write(center);
  delay(1500);
  //test.write(center-servoSubtract);
  delay(1500);
}
