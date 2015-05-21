#include <Servo.h>
#include <AV4Wheel.h>

//space between cars: 8.5 feet
#define NumUltras 3

AV4Wheel car;

Servo front;

const int center = 97;
const int addAngle = 21; //furthest it can turn left (center+) BRUUHH
const int subAngle = 25.5; //furthest it can turn right ^^^ BRUHHHHHH
const int buttonPin = 10;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 125;
const int exaggerate = 2;

int maxDistance = 200;

//Front,back, right
int triggerPins[NumUltras] = {12,9,7};
int echoPins[NumUltras] = {12,9,8};

void setup() {
  //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  pinMode(buttonPin,buttonInputType);
  /*for(int i = 0; i < NumUltras; i++){
    ultras[i].init(11,A3,13,4*3.14);
    ultras[i].initUltra(triggerPins[i],echoPins[i],maxDistance);
  }*/
  car.init(13,11,A2,6, 3.14*10.25);
  car.setServo(center);
  
  Serial.begin(9600);
}

void loop() {
  if(digitalRead(buttonPin)==LOW){
      car.initUltra(triggerPins[2],echoPins[2],maxDistance);
      car.rampMotion(0,maxSpeed,1,20,false);
      //fwd till begin of first car
      car.moveUltra(24,false,false,maxSpeed,center);
      //fwd util end of first car
      car.moveUltra(30,true,false,maxSpeed,center);
      //fwd util begining of second car
      car.moveUltra(24,false,false,maxSpeed,center);
      car.rampMotion(maxSpeed,0,1,20,false);
      car.initUltra(triggerPins[1],echoPins[1],maxDistance);
      //bkwd to halfway between cars
      car.rampMotion(0,maxSpeed/2,1,20,true);
      car.setServo(center-subAngle*0.43);
      car.moveUltra(50,false,true,maxSpeed/2,center-subAngle*0.43);
      //bkwd to end of car
      car.setServo(center+addAngle);
      car.move(true,maxSpeed/2,center+addAngle,20.0,0);
      car.rampMotion(maxSpeed/2,0,1,20,true);
      car.initUltra(triggerPins[0],echoPins[0],maxDistance);
      car.setServo(center-subAngle*0.65);
      car.rampMotion(0,maxSpeed/2,1,20,false);
      car.moveUltra(30,false,false,maxSpeed/2,center-subAngle*0.65);
      car.rampMotion(maxSpeed/2,0,1,20,false);
      car.setServo(center);
  }
  delay(10);
}
