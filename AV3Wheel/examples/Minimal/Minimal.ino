#include <AV3Wheel.h>

AV3Wheel test;

void setup(){
  test.init(9,10,11,12,2);
}

void loop(){
  test.forward(false, 125, 0, 1000);
  test.spin(false, 125, 0, 1000);
  test.circle(false, 160, 100, 0, 1000);
}