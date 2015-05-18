//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>

//Creating 4WheelDrive object
AV4Wheel test;

const int center = 95;
const int addAngle = 20;
const int subAngle = 30;
const int buttonPin = 12;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 125;

void setup(){
  //Initializing 4WheelDrive
  //Parameters: Motor pin a(Dirn), Motor pin b(PWM), Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  test.init(13,11,A3,6, 3.14*12.0);
  test.setServo(center);
  //Attach interrupt on pin 2
  attachInterrupt(0, interruptFunc, RISING);
  
  pinMode(buttonPin,buttonInputType);
  Serial.begin(9600);
}

void loop(){
  if(digitalRead(buttonPin) == LOW){
    test.setServo(center);
    //start pwm, end pwm, step size, delay, distance(in), reverse, ramp down
    test.moveRampMotion(0,maxSpeed,1,20,12.0*20.0,false,true);
    //time(milliseconds);
    test.stopMotion(200);
  }
  Serial.println(test.getInterrupTicks());
}

void interruptFunc(){
  test._interrupEncoderFunc();
}
