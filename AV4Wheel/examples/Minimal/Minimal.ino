#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel test(9,10,11,12,2,3);

void setup(){}

void loop(){
  test.move(false, 125, 90, 0, 1000);
}
