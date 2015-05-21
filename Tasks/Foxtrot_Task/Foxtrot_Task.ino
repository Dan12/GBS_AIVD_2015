#include <SoftwareSerial.h>
#include <Servo.h>
#include <AV4Wheel.h>

int TxD = 5;
int RxD = 4;

SoftwareSerial bluetooth(TxD, RxD);

AV4Wheel aivdCar;

const int center = 97;
const int addAngle = 21;
const int subAngle = 25.5;
const int buttonPin = 10;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 125;
const int exaggerate = 2;

int maxDistance = 200;
int distance = maxDistance;
int stoppingDistance = 30;

int speedChangeDelay = 20;
int speedAt = 0;

char currentChar = 'n';

void setup(){
  //Setup usb serial connection to computer
  Serial.begin(9600);
  Serial.println("Begin");
  //Setup Bluetooth serial connection to android
  bluetooth.begin(115200);
  bluetooth.print("$$$");
  delay(100);
  bluetooth.println("U,9600,N");
  bluetooth.begin(9600);
  bluetooth.setTimeout(20);
  
  //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  aivdCar.init(13,11,A2,6, 3.14*10.25);
  aivdCar.setServo(center);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  aivdCar.rampMotion(0,maxSpeed/3,1,5,false);
  speedAt = maxSpeed/3;
}

void loop()
{
  //Read from bluetooth and write to usb serial
  if(bluetooth.available()){
    char c = bluetooth.read();
    Serial.println(c);
    if(c == 'g' || c == 'y' || c == 'r'){
      currentChar = c;
    }
  }
  if(currentChar == 'g'){
    if(speedAt != maxSpeed){
      aivdCar.rampMotion(speedAt,maxSpeed,1,20,false);
      speedAt = maxSpeed;
    }
  }
  else if(currentChar == 'y'){
    if(speedAt != maxSpeed/2){
      aivdCar.rampMotion(speedAt,maxSpeed/2,1,20,false);
      speedAt = maxSpeed/2;
    }
  }
  else if(currentChar == 'r'){
    if(speedAt != 0){
      aivdCar.rampMotion(speedAt,0,1,20,false);
      speedAt = 0;
    }
  }
}
