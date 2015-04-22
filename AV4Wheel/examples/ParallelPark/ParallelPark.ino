//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel avproto;

int buttonPin = 6;

void setup(){
    //Button pin
    pinMode(buttonPin,INPUT_PULLUP);
    
    //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
    avproto.init(11, 7, 9, 4*3.14);
    //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
    avproto.initUltra(10, 8, 200);
}

void loop(){
    if(digitalRead(buttonPin) == LOW){
        avproto.moveUltra(8, false, false, 230, 90);
        avproto.move(false, 230, 90, 12.0, 0);
        avproto.move(false, 150, 125, 18.0, 0);
        avproto.move(false, 150, 55, 28.0, 0);
        avproto.move(false, 230, 110, 14.0, 0);
    }
}
