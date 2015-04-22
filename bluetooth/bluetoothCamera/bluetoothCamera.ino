#include <SoftwareSerial.h>

int TxD = 2;
int RxD = 3;

SoftwareSerial bluetooth(TxD, RxD);

AV4Wheel avproto;

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
  avproto.init(11,7,9, 4*3.14);
}

void loop()
{
  //Read from bluetooth and write to usb serial
  if(bluetooth.available())
  {
    char c = bluetooth.read();
    Serial.println(c);
    if(c == 'f'){
      avproto.diffMove(false,230,90);
    }
    else if(c == 'r'){
      avproto.diffMove(false,230,180);
    }
    else if(c == 'l'){
      avproto.diffMove(false,230,0);
    }
  }
}
