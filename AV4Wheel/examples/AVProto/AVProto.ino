//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel avproto;

int buttonPin = 6;

void setup(){
    //Button pin
    pinMode(buttonPin,INPUT_PULLUP);
    
    //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
    avproto.init(11,7,9, 4*3.14);
}

void loop(){
    if(digitalRead(buttonPin) == LOW){
        //reverse,duty cycle, servo angle, dist (inches), time (miliseconds) 
        //1 Pin Motor Duty cycle and equivalent directions (slow-fast): 10-70: stop, 170-90: cw, 200-250: ccw
        avproto.move(false, 230, 90, 0, 1000);
        avproto.move(false, 230, 65, 0, 1000);
        avproto.move(false, 230, 90, 0, 1000);
        avproto.move(false, 230, 115, 0, 1000);
    }
}
