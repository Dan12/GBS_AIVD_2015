//This is the gps code that will send car to the gotolat and gotolong coordinates

#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define Addr 0x1E 

#define RXPIN 0
#define TXPIN 1

#define GPSBAUD 4800

float latitude, longitude;

float gotolat = 42.090070;
float gotolong = -87.850336;

float curangle;

float xpos = 0.0;
float ypos = 0.0;

float xTrim = 70;
float yTrim = 109;
float angleTrim = 180;

boolean move1 = false;

TinyGPS gps;

SoftwareSerial uart_gps(RXPIN, TXPIN);

void getgps(TinyGPS &gps);

void setup()
{
  Serial.begin(115200);

  Wire.begin();

  Wire.beginTransmission(Addr); 
  Wire.write(byte(0x02));
  Wire.write(byte(0x00));
  Wire.endTransmission();

  uart_gps.begin(GPSBAUD);

  pinMode(12,INPUT);

  Serial.println("");
  Serial.println("GPS Shield QuickStart Example Sketch v12");
  Serial.println("       ...waiting for lock...           ");
  Serial.println("");
}

void loop()
{

  if(uart_gps.available())
  {
    int c = uart_gps.read();
    if(gps.encode(c))
    {
      Serial.print("Go To Lat/Long: ");
      Serial.print(gotolat,8);
      Serial.print(", ");
      Serial.println(gotolong,8);
      getgps(gps);
      xpos = (float)(gotolong-longitude);
      ypos = (float)(gotolat-latitude);
      convertdeg();
      float gotoangle = (float)(atan2((float)ypos,(float)xpos) * (180 / PI) + 180);
      gotoangle -= 180;
      if (gotoangle<0){
        gotoangle+=360;
      }
      curangle = getangle();
      Serial.print("X Distance(feet): ");
      Serial.println(xpos);
      Serial.print("Y Distance(feet): ");
      Serial.println(ypos);
      Serial.print("Goto angle(degrees): ");
      Serial.println(gotoangle);

      Serial.print("Degrees to turn: ");
      if (gotoangle>curangle){
        Serial.print(gotoangle-curangle);
        Serial.println(" Clockwise");
      }
      else {
        Serial.print(curangle-gotoangle);
        Serial.println(" Counter Clockwise");
      }
      
      //distance in feet
      float distance = sqrt((((float)(xpos))*((float)(xpos)))+(((float)(ypos))*((float)(ypos))));
      Serial.print("Distance (inches): ");
      Serial.println(distance*12.0);
      
      Serial.println("\\");
      
      move1 = false;
      delay(1000);
    }
  }
}

void getgps(TinyGPS &gps)
{
  gps.f_get_position(&latitude, &longitude);
  // You can now print variables latitude and longitude
  Serial.print("Lat/Long: "); 
  Serial.print(latitude,8); 
  Serial.print(", "); 
  Serial.println(longitude,8);
}

int getangle(){
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
  Serial.print("X=");
  Serial.print(x);
  Serial.print(", Y=");
  Serial.print(y);
  Serial.print(", Z=");
  Serial.println(z);

  double angle = atan2((double)y,(double)x) * (180 / PI) + 180;
  
  angle += angleTrim;
  if (angle < 0){
    angle += 360;
  }
  if (angle >= 360){
    angle -= 360;
  }

  Serial.print("angle: ");
  Serial.println(angle);

  return angle;
}

int convertdeg(){
  xpos*=((float)365165.33582);
  ypos*=((float)362776.86915);
}

int pause(){
  //analogWrite(speedPin, 0);
  delay(500);
}
