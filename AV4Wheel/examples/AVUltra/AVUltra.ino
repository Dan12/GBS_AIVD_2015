//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel avproto;

int buttonPin = 5;

void setup(){
    //Button pin
    pinMode(buttonPin,INPUT_PULLUP);
    
    //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
    avproto.init(11,7,9, 10.0);
    //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
    avproto.initUltra(8,10, 200);
}

void loop(){
    if(digitalRead(buttonPin) == LOW){
        //1 Pin Motor Duty cycle and equivalent directions (slow-fast): 10-70: stop, 170-90: cw, 200-250: ccw
        //Will move in direction specified at duty cycle of 230 and servo at 90 for 1 second
        //Parameters: reverse,duty cycle, servo angle, dist (inches), time (miliseconds) 
        avproto.move(false, 230, 90, 0, 1000);
        
        //Will move in direction specified at duty cycle of 230 and servo at 90 while objects are greater than 30 inches away
        //Parameters: distance(in), move while less than d, reverse motion, duty cycle, servo angle
        avproto.moveUltra(8, false, false, 230, 90);
    }
}