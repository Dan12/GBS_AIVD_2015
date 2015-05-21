#include <Wire.h>

#define Addr 0x1E               // 7-bit address of HMC5883 compass

int xTrim = 0; //70
int yTrim = 0; //109
int angleTrim = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(20);
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
  if (Serial.available() > 0) {
    xTrim = Serial.parseInt();
    yTrim = Serial.parseInt();
    angleTrim = Serial.parseInt();
    delay(100);
  }
  else{
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
    
    y += yTrim;
    x += xTrim;
    
    // Print raw values
  //  Serial.print("X=");
  //  Serial.print(x);
  //  Serial.print(", Y=");
  //  Serial.print(y);
  //  Serial.print(", Z=");
  //  Serial.println(z);
    
    double angle = atan2((double)y,(double)x) * (180 / PI) + 180;
    
    angle += angleTrim;
    if (angle >= 360){
      angle-=360;
    }
    if(angle < 0){
      angle += 360;
    }
  
    //Serial.print("angle: ");
    Serial.print(angle);
    Serial.println(","+(String)x+","+(String)y+","+(String)z);
    
    delay(50);
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println("A");   // send a capital A
  delay(300);
  }
}
