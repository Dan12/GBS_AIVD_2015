//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel avpro;

int buttonPin = 5;

void setup(){
    //Button pin
    pinMode(buttonPin,INPUT);
    
    //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
    avpro.init(11,7,9, 10.0);
}

void loop(){
    if(digitalRead(buttonPin) == LOW){
        //reverse,duty cycle, servo angle, dist (inches), time (miliseconds) 
        //Duty cycle: 10-70: stop, 170-90: cw, 200-250: ccw
        test.move(false, 240, 90, 0, 1000);
        test.move(false, 240, 65, 0, 1000);
        test.move(false, 240, 115, 0, 1000);
    }
}