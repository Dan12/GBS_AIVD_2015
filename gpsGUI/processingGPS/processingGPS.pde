import com.modestmaps.*;
import com.modestmaps.core.*;
import com.modestmaps.geo.*;
import com.modestmaps.providers.*;

import processing.serial.*;

InteractiveMap map;

Serial myPort;
String val;

float lat = 0;
float log = 0;
float curAngle = 0;
float goToAngle = 0;
float distance = 0;
float gtlat = 0;
float gtlog = 0;

boolean setCenter = false;

boolean gui = true;

void setup() {
  size(640, 480);

  String portName = Serial.list()[3];
  println(portName);
  myPort = new Serial(this, portName, 115200);
  
  String template = "http://{S}.mqcdn.com/tiles/1.0.0/osm/{Z}/{X}/{Y}.png";
  String[] subdomains = new String[] { "otile1", "otile2", "otile3", "otile4" }; // optional
  map = new InteractiveMap(this, new TemplatedMapProvider(template, subdomains));

  map.setCenterZoom(new Location(lat, log), 18);  
}

void draw()
{
  //background(255);
  if(!setCenter)
    map.setCenterZoom(new Location(lat, log), 18);  
  if(lat != 0)
    setCenter = true;
  
  if ( myPort.available() > 0) 
  {  // If data is available,
  val = myPort.readStringUntil('\\');         // read it and store it in val
  } 
  if(val != null){
    //println(val);
    String vals[] = val.split("\n");
    String latlong = "";
    if(vals[1].length() > 10 && vals.length >= 9){
      String gtlatLong = vals[1].substring(16);
      int i = gtlatLong.indexOf(",");
      if(i != -1){
        gtlat = Float.parseFloat(gtlatLong.substring(0,i));
        gtlog = Float.parseFloat(gtlatLong.substring(i+2));
      }
      latlong = vals[2].substring(10);
      i = latlong.indexOf(",");
      if(i != -1){
        lat = Float.parseFloat(latlong.substring(0,i));
        log = Float.parseFloat(latlong.substring(i+2));
      }
      String angle = vals[4];
      i = angle.indexOf(":");
      if(i != -1){
        curAngle = Float.parseFloat(angle.substring(i+2));
      }
      String goAngle = vals[7];
      i = goAngle.indexOf(":");
      if(i != -1){
        goToAngle = Float.parseFloat(goAngle.substring(i+2));
      }
      String d = vals[9];
      i = d.indexOf(":");
      if(i != -1){
        distance = Float.parseFloat(d.substring(i+2));
      }
    }
    
    Point2f p = map.locationPoint(new Location(gtlat,gtlog));
    Point2f p1 = map.locationPoint(new Location(lat,log));
    float dCX = p.x - p1.x;
    float dCY = p.y - p1.y;
    
    map.draw();
    fill(255,0,0);
    triangle(p1.x+cos(radians(curAngle-200-90))*15,p1.y+sin(radians(curAngle-200-90))*15,p1.x+cos(radians(curAngle-90))*15,p1.y+sin(radians(curAngle-90))*15,p1.x+cos(radians(curAngle+200-90))*15,p1.y+sin(radians(curAngle+200-90))*15);
    fill(0);
    line(p.x, p.y, p1.x, p1.y);
    if(gui){
      textSize(16);
      textAlign(CENTER);
      text("Distance to go: "+(distance/12)+" feet", width/2, height-30);
      if(curAngle-goToAngle > 0)
        text("Turn "+(curAngle-goToAngle)+" Degrees Counter Clockwise", width/2, height-10);
      else
        text("Turn "+(goToAngle-curAngle)+" Degrees Clockwise", width/2, height-10);
      Location l = map.pointLocation(mouseX, mouseY);
      text("Mouse location: "+l, width/2, 14);
      text("Current Angle: "+curAngle, width/2, 34);
    }
    //println(latlong);
    print("Lat: "+lat); //print it out in the console
    println("   Long: "+log);
    print("Go To Lat: "+gtlat);
    println("   Long: "+gtlog);
    println("Current Angle: "+curAngle);
    println("Go To Angle: "+goToAngle);
    println("Distance (feet): "+(distance/12));
    println("----------");
  }
}

void mouseDragged(){
  map.mouseDragged();
}

void keyPressed(){
  if(key == 'h' || key == 'H')
    gui = !gui;
}

