//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>
#include <NewPing.h>

//Creating 4WheelDrive object
AV4Wheel test;

void setup(){
  //Initializing 4WheelDrive
  //If 1 motor:
  //If 2 Motor Pins:
  //Parameters: Motor pin a, Motor pin b, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  test.init(9,10,2,3, 10.0);
  //If 1 Motor Pin:
  //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  test.init(9,2,3, 10.0);
  //If 2 motors:
  //Parameters: Motor 1 pin a, Motor 1 pin b, Motor 2 pin a, Motor 2 pin b, Encoder pin, Steering Servo pin, Wheel Circumfrence (in inches)
  test.init(9,10,11,12,2,3,10.0);
}

void loop(){
  //Move method for both 1 motor and 2 motor setups
  //Parameters: Invert Motion, Speed/PMW Cycle to motor(0-255), Steering Servo angle, distance (in inches), time (in miliseconds)
  //If the distance is 0, then it will move for the specified time and vice versa; if both time and distance are set to 0 or not set to 0, then nothing will happen
  test.move(false, 125, 90, 0, 1000);
}
