//Include 3WheelDrive library
#include <AV3Wheel.h>

//Initialize 3Wheel Drive Object
AV3Wheel test;

void setup(){
  //Setup 3WheelDrive Object
  //Parameters: Motor 1 pin a, Motor 1 pin b, Motor 2 pin a, Motor 2 pin b, Encoder pin, Wheel Circumfrence (in inches)
  test.init(9,10,11,12,2,10.0);
}

void loop(){
  //Forward motion method
  //Parameters: Invert Motion, Speed (0-255), distance (in inches), time (in milliseconds)
  //If time is 0, it will move for the specified distance and vice verca. If both are 0 or both are not 0, nothing will happen
  test.forward(false, 125, 0, 1000);
  
  //Spin on a point motion method
  //Parameters: Invert Motion, Speed (0-255), distance (in inches), time (in milliseconds)
  //If time is 0, it will move for the specified distance and vice verca. If both are not 0, nothing will happen
  test.spin(false, 125, 0, 1000);
  
  //Circle motion method
  //Parameters: Invert Motion, First Motor Speed (0-255), First Motor Speed (0-255), distance (in inches), time (in milliseconds)
  //If time is 0, it will move for the specified distance and vice verca. If both are not 0, nothing will happen
  test.circle(false, 160, 100, 0, 1000);
}
