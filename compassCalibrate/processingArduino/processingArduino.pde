import controlP5.*;

import processing.serial.*;

Serial myPort;
String val;
boolean firstContact = false;

boolean editX = false;
boolean editY = false;
boolean editA = false;
String xTrim = "40";
String yTrim = "130";
String aTrim = "-18";
boolean firstKeyPress = false;

void setup() {
  smooth();
  size(300, 350);

  String portName = Serial.list()[3];
  println(portName);
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
}

void draw() {
}

void mouseClicked(){
  if(mouseX < 100){
    editX = !editX;
    editY = false;
    editA = false;
    firstKeyPress = false;
  }
  else if(mouseX < 200){
    editA = !editA;
    editY = false;
    editX = false;
    firstKeyPress = false;
  }
  else{
    editY = !editY;
    editX = false;
    editA = false;
    firstKeyPress = false;
  }
}

void keyPressed(){
  if(editX && !firstKeyPress){
    xTrim = "";
    firstKeyPress = true;
  }
  if(editY && !firstKeyPress){
    yTrim = "";
    firstKeyPress = true;
  }
  if(editA && !firstKeyPress){
    aTrim = "";
    firstKeyPress = true;
  }
  if(editA)
    aTrim+=key;
  if(editX)
    xTrim+=key;
  if(editY)
    yTrim+=key;
}

void serialEvent( Serial myPort) {
  //put the incoming data into a String - 
  //the '\n' is our end delimiter indicating the end of a complete packet
  val = myPort.readStringUntil('\n');
  //make sure our data isn't empty before continuing
  if (val != null) {
    //trim whitespace and formatting characters (like carriage return)
    val = trim(val);
    println(val);

    //look for our 'A' string to start the handshake
    //if it's there, clear the buffer, and send a request for data
    if (firstContact == false) {
      if (val.equals("A")) {
        myPort.clear();
        firstContact = true;
        myPort.write("A");
        println("contact");
      }
    } else { //if we've already established contact, keep getting and parsing data
      println("Val: "+val);
      float angle;
      String[] vals = new String[5];
      if(!val.equals("A")){
        //println("did this");
        vals = val.split(",");
        angle = Float.parseFloat(vals[0]);
      }
      else{
        for(int i = 0; i < 5; i++)
          vals[i] = "0";
        angle = 0;
      }
      background(255);
      fill(0);
      textSize(20);
      textAlign(CENTER);
      text(vals[0], 150, 296);
      stroke(0);
      fill(255);
      ellipse(150, 150, 200, 200);
      fill(255, 0, 0);
      triangle(cos(radians(angle-90))*25+150, sin(radians(angle-90))*25+150, cos(radians(angle))*100+150, sin(radians(angle))*100+150, cos(radians(angle+90))*25+150, sin(radians(angle+90))*25+150);
      fill(0);
      triangle(cos(radians(angle-90))*25+150, sin(radians(angle-90))*25+150, cos(radians(angle+180))*100+150, sin(radians(angle+180))*100+150, cos(radians(angle+90))*25+150, sin(radians(angle+90))*25+150);
      fill(150);
      ellipse(150, 150, 50, 50);
      fill(0);
      textSize(14);
      for (int i = 0; i < 360; i += 45) {
        text(i+"", cos(radians(i+180))*115+150, sin(radians(i+180))*115+156);
      }
      if(editX)
        fill(255);
      else
        fill(100);
      rect(18, 304, 50, 20); 
      if(editY)
        fill(255);
      else
        fill(100);
      rect(238, 304, 50, 20); 
      if(editA)
        fill(255);
      else
        fill(100);
      rect(118, 304, 50, 20);
      fill(0);
      textAlign(LEFT);
      text("X: "+vals[1], 20, 14);
      text("Y: "+vals[2], 120, 14);
      text("Z: "+vals[3], 240, 14);
      text(xTrim, 20, 320);
      text(yTrim, 240, 320);
      text(aTrim, 120, 320);
      text("X Trim", 20, 340);
      text("Y Trim", 242, 340);
      text("Angle Trim", 113, 340);
      println(xTrim+","+yTrim+","+aTrim);
      myPort.write(xTrim+","+yTrim+","+aTrim);

      //    if (mousePressed == true) 
      //    {                           //if we clicked in the window
      //      myPort.write('1');        //send a 1
      //      println("1");
      //    }

      // when you've parsed the data you have, ask for more:
      //myPort.write("A");
    }
  }
}

