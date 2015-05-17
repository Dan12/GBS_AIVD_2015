//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel aivdCar;

const int center = 95;
const int addAngle = 20;
const int subAngle = 30;
const int buttonPin = 12;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 125;

void setup(){
    //Button pin
    pinMode(buttonPin,buttonInputType);
    
    //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
    aivdCar.init(13,11,5,6, 3.14*12.0);
    //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
    aivdCar.initUltra(10, 10, 200);
}

void loop(){
    if(digitalRead(buttonPin) == LOW){ 
        //Will move in direction specified at duty cycle of 230 and servo at 90 while objects are greater than 30 inches away
        //Parameters: distance(in), move while less than d, reverse motion, duty cycle, servo angle
        aivdCar.rampMotion(0,maxSpeed,1,5,false);
        aivdCar.moveUltra(24, false, false, maxSpeed, center);
        aivdCar.rampMotion(maxSpeed,0,1,5,false);
    }
}
