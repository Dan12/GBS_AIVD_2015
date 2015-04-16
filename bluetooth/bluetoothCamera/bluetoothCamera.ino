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
    char c = bluetooth.read();
    Serial.println(c);
    Serial.println(c == 'f');
  }
}
