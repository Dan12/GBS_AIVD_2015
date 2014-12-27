#include <Wire.h>

#define Addr 0x1E               // 7-bit address of HMC5883 compass

void setup() {
  Serial.begin(9600);
  delay(100);                   // Power up delay
  Wire.begin();
  
  // Set operating mode to continuous
  Wire.beginTransmission(Addr); 
  Wire.write(byte(0x02));
  Wire.write(byte(0x00));
  Wire.endTransmission();
  
  establishContact();
}

void loop() {
  int x, y, z;

  // Initiate communications with compass
  Wire.beginTransmission(Addr);
  Wire.write(byte(0x03));       // Send request to X MSB register
  Wire.endTransmission();

  Wire.requestFrom(Addr, 6);    // Request 6 bytes; 2 bytes per axis
  if(Wire.available() <=6) {    // If 6 bytes available
    x = Wire.read() << 8 | Wire.read();
    z = Wire.read() << 8 | Wire.read();
    y = Wire.read() << 8 | Wire.read();
  }
  
  y += 150.0;
  x += 40.0;
  
  // Print raw values
//  Serial.print("X=");
//  Serial.print(x);
//  Serial.print(", Y=");
//  Serial.print(y);
//  Serial.print(", Z=");
//  Serial.println(z);
  
  double angle = atan2((double)y,(double)x) * (180 / PI) + 180;
  
//  angle+=22;
//  if (angle>360){
//    angle-=360;
//  }

  //Serial.print("angle: ");
  Serial.println(angle);
  
  //delay(500);
}

void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println("A");   // send a capital A
  delay(300);
  }
}
