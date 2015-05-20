//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>

//Creating 4WheelDrive object
AV4Wheel test;

const int center = 97;
const int addAngle = 21;
const int subAngle = 25.5;
const int buttonPin = 10;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 125;
const int exaggerate = 2;

void setup(){
  //Initializing 4WheelDrive
  //Parameters: Motor pin a(Dirn), Motor pin b(PWM), Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  test.init(13,11,5,6, 3.14*12.0);
  test.setServo(center);
  
  pinMode(buttonPin,buttonInputType);
}

void loop(){
  if(digitalRead(buttonPin) == LOW){
    test.setServo(center);
    //start pwm, end pwm, step size, delay, distance(in), reverse, ramp down
    test.moveRampMotion(0,maxSpeed,1,20,12.0*17.0,false,false);
    //Parameters: Invert Motion, Speed/PMW Cycle to motor(0-255), Steering Servo angle, distance (in inches), time (in miliseconds)
    //If the distance is 0, then it will move for the specified time and vice versa; if both time and distance are set to 0 or not set to 0, then nothing will happen
    test.move(false,maxSpeed,center+addAngle,12.0*7.6,0);
    test.move(false,maxSpeed,center-exaggerate,12.0*10.0,0);
    test.move(false,maxSpeed,center+addAngle,12.0*15.1,0);
    test.move(false,maxSpeed,center-exaggerate,12.0*10.0,0);
    //start,end,step,delay,reverse
    test.rampMotion(maxSpeed,0,1,20,false);
    //time(milliseconds);
    test.stopMotion(200);
  }
}
