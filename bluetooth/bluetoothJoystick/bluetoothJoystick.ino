#include <SoftwareSerial.h>
#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel avproto;

int TxD = 2;
int RxD = 3;

int maxDistance = 200;
int distance = maxDistance;

int angle = 90;
int knobX = 0;
int knobY = 0;

SoftwareSerial bluetooth(TxD, RxD);

Servo ultraServo;
int ultraServoPin = 4;

boolean returnMes = false;

boolean isProto = true;

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
  bluetooth.setTimeout(30);
  
  //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  avproto.init(11,7,9, 4*3.14);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  avproto.initUltra(10, 8, maxDistance);
  
  ultraServo.attach(ultraServoPin);
  ultraServo.write(90);
}

void loop()
{
  //Read from bluetooth and write to usb serial
  if(bluetooth.available())
  {
    int data1 = bluetooth.parseInt();
    Serial.println(data1);
    int data2 = bluetooth.parseInt();
    Serial.println(data2);
    int data3 = bluetooth.parseInt();
    Serial.println(data3);
    angle = data1;
    knobX = data2;
    knobY = data3*-1;
    Serial.println("*");
    Serial.println(angle);
    Serial.println(knobX);
    Serial.println(knobY);
    Serial.println("*");
    returnMes = true;
    setMotion();
  }
  else if(returnMes){
    int distance = avproto.getUltraIn();
    if(distance == 0)
      distance = maxDistance;
    sendInt(distance);
    Serial.println(distance);
    returnMes = false;
  }
}

void setMotion(){
  int moveSpeed = constrain(map(knobY,-200,200,-255,255),-255,255);
  int moveAngle = constrain(map(knobX,-200,200,55,135),55,135);
  boolean reverse = moveSpeed < 0;
  moveSpeed = abs(moveSpeed);
  
  if(isProto){
    if(moveSpeed != 0){
      if(reverse){
        moveSpeed = map(moveSpeed,0,255,170,90);
      }
      else{
        moveSpeed = map(moveSpeed,0,255,200,250);
      }
    }
    else{
      moveSpeed = 10;
    }
    reverse = false;
  }
  
  avproto.setServo(moveAngle);
  avproto.diffMove(reverse,moveSpeed);
  
  ultraServo.write(angle);
}

void sendInt(int d){
  String str = String(d);
  char b[2];
  for(int i = 0; i < str.length(); i++){
    String dataSend = str.substring(i,i+1);
    dataSend.toCharArray(b,2);
    bluetooth.print(b);
  }
  String endStr = "*";
  char e[2];
  endStr.toCharArray(e,2);
  bluetooth.print(e);
}
