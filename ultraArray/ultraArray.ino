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

int triggerPins[NumUltras] = {1,12,5,10};
int echoPins[NumUltras] = {2,13 ,6,8);

//Max distance in cm
int maxDistance = 200;

void setup() {
  //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  car.init(11,7,9, 4*3.14);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  for(int i = 0; i < NumUltras; i++){
    ultras[i].initUltra(triggerPins[i],echoPins[i],maxDistance);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int distances[NumUltras];
  for(int i = 0; i < NumUltras; i++){
    distances[i] = ultras[i].getUltraIn();
    Serial.print(i); Serial.print(": "); Serial.println(distances[i]);
    delay(10);
  }
  
}
