//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>

//Creating 4WheelDrive object
AV4Wheel test;

const int center = 91;
const int addAngle = 24;
const int subAngle = 28.5;
const int buttonPin = 10;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 125;
const int exaggerate = 1;

void setup(){
  //Initializing 4WheelDrive
  //Parameters: Motor pin a(Dirn), Motor pin b(PWM), Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  test.init(13,11,A2,6,3.14*10.25);
  test.setServo(97);
  
  pinMode(buttonPin,buttonInputType);
}

void loop(){
  if(digitalRead(buttonPin) == LOW){
    test.setServo(97);
    //start pwm, end pwm, step size, delay, distance(in), reverse, ramp down
    test.rampMotion(0,maxSpeed,1,20,false);
    test.setServo(center);
    test.move(false,maxSpeed,center,12.0*38.0,0);
    //Parameters: Invert Motion, Speed/PMW Cycle to motor(0-255), Steering Servo angle, distance (in inches), time (in miliseconds)
    //If the distance is 0, then it will move for the specified time and vice versa; if both time and distance are set to 0 or not set to 0, then nothing will happen
    test.move(false,maxSpeed,center+addAngle,12.0*7.75,0);
    test.move(false,maxSpeed,center-exaggerate,12.0*30.0,0);
    test.move(false,maxSpeed,center+addAngle,12.0*15.4,0);
    test.move(false,maxSpeed,center-exaggerate,12.0*15.0,0);
    test.move(false,maxSpeed,center-subAngle,12.0*7.45,0);
    test.move(false,maxSpeed,center+exaggerate,12.0*18.0,0);
    test.move(false,maxSpeed,center-subAngle,12.0*15.4,0);
    test.move(false,maxSpeed,center+exaggerate,12.0*10.0,0);
    test.move(false,maxSpeed,center+addAngle,12.0*7.75,0);
    test.move(false,maxSpeed,center-exaggerate,12.0*10.0,0);
    test.move(false,maxSpeed,center+addAngle,12.0*7.75,0);
    test.move(false,maxSpeed,center-exaggerate,12.0*17.0,0);
    //start,end,step,delay,reverse
    test.rampMotion(maxSpeed,0,1,20,false);
    //time(milliseconds);
    test.stopMotion(200);
  }
}
