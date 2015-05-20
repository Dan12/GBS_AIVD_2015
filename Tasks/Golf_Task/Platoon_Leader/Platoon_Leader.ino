const int wheelSpeed = 100;
const int speedPin = 5;

const int lBackPin = 7;
const int rBackPin = 10;
const int lFrontPin = 8;
const int rFrontPin = 9;

//initialize wheel diameter
float diam = 6.5;

const int buttonPin = 12;

boolean move1 = false;

int moveTimes = 10;

const int delayRunTime = 2000;

const int waitTime = 1000;

void setup() {
  randomSeed(analogRead(0));
  
  // put your setup code here, to run once:
  pinMode(speedPin, OUTPUT);
  pinMode(lBackPin, OUTPUT);
  pinMode(rBackPin, OUTPUT);
  pinMode(lFrontPin, OUTPUT);
  pinMode(rFrontPin, OUTPUT);
  
  pinMode(12,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(12) == LOW && move1 == false){
    move1 = true;
  }
  
  if (move1 == true){
    int timesMoved = 0;
    while(timesMoved < moveTimes){
      int action = random(0,2);
      //turn right
      if(action == 0){
        moveWheels(false,true,false,true);
      }
      //turn left
      else{
        moveWheels(true,false,true,false);
      }
      //forward
      moveWheels(true,true,true,true);
      action = random(0,2);
      while(action == 0){
        moveWheels(true,true,true,true);
        action = random(0,2);
      }
      timesMoved++;
    }
  }
}

void moveWheels(boolean rf, boolean lf, boolean rb, boolean lb){
  digitalWrite(lBackPin,lb);
  digitalWrite(lFrontPin,lf);
  digitalWrite(rFrontPin,rf);
  digitalWrite(rBackPin,rb);
  analogWrite(speedPin, wheelSpeed);
  
  delay(delayRunTime);
  pause();
  delay(waitTime);
}

void pause(){
  analogWrite(speedPin, 0);
  delay(200);
}
