#include <SoftwareSerial.h>
#include <Servo.h>
#include <AV4Wheel.h>

int TxD = 2;
int RxD = 3;

SoftwareSerial bluetooth(TxD, RxD);

AV4Wheel aivdCar;

const int center = 95;
const int addAngle = 20;
const int subAngle = 30;
const int buttonPin = 12;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 125;

int maxDistance = 200;
int distance = maxDistance;
int stoppingDistance = 60;

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
  aivdCar.init(13,11,5,6, 3.14*12.0);
  aivdCar.setServo(center);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  aivdCar.initUltra(10, 10, maxDistance);
  aivdCar.rampMotion(0,maxSpeed/2,1,5,false);
}

void loop()
{
  //Read from bluetooth and write to usb serial
  if(bluetooth.available()){
    char c = bluetooth.read();
    Serial.println(c);
    if(c == 'f'){
      aivdCar.setServo(center);
    }
    else if(c == 'r'){
      aivdCar.setServo(center-subAngle);
    }
    else if(c == 'g'){
      aivdCar.setServo(center-subAngle/2);
    }
    else if(c == 'l'){
      aivdCar.setServo(center+addAngle);
    }
    else if(c == 'd'){
      aivdCar.setServo(center+addAngle/2);
    }
    if(c == 's'){
      aivdCar.diffMove(false,0);
    }
    else if(distance > stoppingDistance){
      aivdCar.diffMove(false,maxSpeed/2);
    }
  }
  distance = aivdCar.getUltraIn();
}
