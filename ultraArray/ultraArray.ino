#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel car;

#define NumUltras 4

AV4Wheel ultras[NumUltras] = {
  AV4Wheel(),
  AV4Wheel(),
  AV4Wheel(),
  AV4Wheel()
};

int triggerPins[NumUltras] = {1,3,5,8};
int echoPins[NumUltras] = {2,4,6,10};

//Max distance in cm
int maxDistance = 200;

void setup() {
  //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  car.init(11,7,9, 4*3.14);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  for(int i = 0; i < NumUltras; i++){
    ultras[i].initUltra(triggerPins[i],echoPins[i],maxDistance);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int distances[NumUltras];
  for(int i = 0; i < NumUltras; i++){
    distances[i] = ultras[i].getUltraIn();
  }
}
