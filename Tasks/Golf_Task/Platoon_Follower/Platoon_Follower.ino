#include <SoftwareSerial.h>
#include <Servo.h>
#include <AV4Wheel.h>

int TxD = 5;
int RxD = 4;

SoftwareSerial bluetooth(TxD, RxD);

AV4Wheel aivdCar;

Servo front;

const int center = 97;
const int addAngle = 21;
const int subAngle = 25.5;
const int buttonPin = 10;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 80;
const int exaggerate = 2;

int maxDistance = 200;
int distance = maxDistance;
int stoppingDistance = 30;

boolean atTopSpeed = false;
int speedAt = 0;
int speedChange = 1;
int speedChangeTimes = 4;
int speedChangeDelay = 15;

boolean isMoving = false;

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
  aivdCar.initUltra(12, 12, maxDistance);
  pinMode(buttonPin,buttonInputType);
  front.attach(A3);
  front.write(90);
}

void loop()
{
  if(digitalRead(buttonPin) == LOW){
    isMoving = true;
    aivdCar.rampMotion(0,maxSpeed,1,15,false);
    atTopSpeed = true;
    speedAt = maxSpeed;
  }
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
    
    if(isMoving){
      //go
      if(distance > stoppingDistance && c != 's'){
        if(atTopSpeed)
          aivdCar.diffMove(false,maxSpeed);
        else{
          for(int i = 0; i < speedChangeTimes; i++){
            speedAt+=speedChange;
            if(speedAt>maxSpeed)
              speedAt = maxSpeed;
            aivdCar.diffMove(false,speedAt);
            delay(speedChangeDelay);
          }
          if(speedAt >= maxSpeed)
            atTopSpeed = true;
        }
      }
      //stop
      else{
        if(!atTopSpeed)
          aivdCar.diffMove(false,0);
        else{
          for(int i = 0; i < speedChangeTimes; i++){
            speedAt-=speedChange;
            if(speedAt<0)
              speedAt = 0;
            aivdCar.diffMove(false,speedAt);
            delay(speedChangeDelay);
          }
          if(speedAt <= 0)
            atTopSpeed = false;
        }
      }
    }
  }
  distance = aivdCar.getUltraIn();
}
