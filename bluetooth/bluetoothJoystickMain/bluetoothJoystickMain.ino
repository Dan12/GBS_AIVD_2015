#include <SoftwareSerial.h>
#include <Servo.h>
#include <AV4Wheel.h>

AV4Wheel aivdCar;

int TxD = 5;
int RxD = 4;

int maxDistance = 200;
int distance = maxDistance;

int angle = 90;
int knobX = 0;
int knobY = 0;

int prevSpeed = 0;
int maxSpeedUp = 3;
boolean prevReverse = false;

int distScale = 2;

SoftwareSerial bluetooth(TxD, RxD);

Servo ultraServo;
int ultraServoPin = A3;

boolean returnMes = false;

const int center = 97;
const int addAngle = 21;
const int subAngle = 25.5;
const int buttonPin = 10;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 150;
const int exaggerate = 2;

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
  aivdCar.init(13,11,A2,6, 3.14*10.25);
  aivdCar.setServo(center);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  aivdCar.initUltra(12, 12, maxDistance);
  
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
    knobX = data2*-1;
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
    int distance = aivdCar.getUltraIn();
    if(distance == 0)
      distance = maxDistance;
    distance*=distScale;
    sendInt(distance);
    Serial.println(distance);
    returnMes = false;
  }
}

void setMotion(){
  int moveSpeed = constrain(map(knobY,-200,200,-maxSpeed,maxSpeed),-maxSpeed,maxSpeed);
  int moveAngle = center;
  if(knobX < 0)
    moveAngle = constrain(map(knobX,-150,0,center-subAngle,center),center-subAngle,center);
  else
    moveAngle = constrain(map(knobX,0,150,center,center+addAngle),center,center+addAngle);  
  if(abs(moveSpeed-prevSpeed) > maxSpeedUp){
    if(moveSpeed-prevSpeed < 0)
      moveSpeed = prevSpeed-maxSpeedUp;
    else 
      moveSpeed = prevSpeed+maxSpeedUp;
  }
  prevSpeed = moveSpeed;
  boolean reverse = moveSpeed < 0;
  moveSpeed = abs(moveSpeed);
  
  aivdCar.setServo(moveAngle);
  aivdCar.diffMove(reverse,moveSpeed);
  
  angle = constrain(angle,45,135);
  
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
