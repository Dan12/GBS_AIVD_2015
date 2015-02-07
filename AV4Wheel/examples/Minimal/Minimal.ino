#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel test;

void setup(){
  test.init(9,10,11,12,2,3);
}

void loop(){
  test.move(false, 125, 90, 0, 1000);
}
