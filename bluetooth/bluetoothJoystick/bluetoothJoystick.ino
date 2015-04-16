#include <SoftwareSerial.h>

int TxD = 2;
int RxD = 3;

int distance = 100;

int angle = 90;
int knobX = 0;
int knobY = 0;

SoftwareSerial bluetooth(TxD, RxD);

boolean returnMes = false;

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
  }
  else if(returnMes){
    int distanceAdd = random(11)-5;
    distance += distanceAdd;
    if(distance > 170)
      distance = 170;
    else if(distance < 50)
      distance = 50;
    sendInt(distance);
    Serial.println(distance);
    returnMes = false;
  }
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
