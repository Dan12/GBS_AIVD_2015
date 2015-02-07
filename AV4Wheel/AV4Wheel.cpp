/*
  AV4Wheel-4Wheel Drive library for AIVD
*/

#include "Arduino.h"
#include "AV4Wheel.h"

#define WHEEL_CIRCUMFRENCE 10
#define TICKS_PER_ROTATION 90

AV4Wheel::AV4Wheel(){}

void AV4Wheel::init(int m1a, int m1b, int m2a, int m2b, int ep, int sp){
    _motor1A = m1a;
    _motor1B = m1b;
    _motor2A = m2a;
    _motor2B = m2b;
    _encoderPin = ep;
    _servoPin = sp;
    
    _hasDifferential = false;
    
    _encoderPrevVal = LOW;
    
    pinMode(_motor1A,OUTPUT);
    pinMode(_motor1B,OUTPUT);
    pinMode(_motor2A,OUTPUT);
    pinMode(_motor2B,OUTPUT);
    
    pinMode(_encoderPin,INPUT);
    
    _steeringServo.attach(_servoPin);
}

void AV4Wheel::init(int ma, int mb, int ep, int sp){
    _motor1A = ma;
    _motor1B = mb;
    _encoderPin = ep;
    _servoPin = sp;
    
    _hasDifferential = true;
    
    _encoderPrevVal = LOW;
    
    pinMode(_motor1A,OUTPUT);
    pinMode(_motor1B,OUTPUT);
    pinMode(_motor2A,OUTPUT);
    pinMode(_motor2B,OUTPUT);
    
    pinMode(_encoderPin,INPUT);
    
    _steeringServo.attach(_servoPin);
}

void AV4Wheel::move(boolean i, int s, int deg, float d, int t){
    if(i){
        if(_hasDifferential){
            digitalWrite(_motor1A, LOW);
            digitalWrite(_motor1B, s);
        }
        else{
            digitalWrite(_motor1A, LOW);
            digitalWrite(_motor1B, s);
            
            digitalWrite(_motor2A,LOW);
            digitalWrite(_motor2B,s);
        }
    }
    else{
        if(_hasDifferential){
            digitalWrite(_motor1A, s);
            digitalWrite(_motor1B, LOW);
        }
        else{
            digitalWrite(_motor1A, s);
            digitalWrite(_motor1B, LOW);
            
            digitalWrite(_motor2A,s);
            digitalWrite(_motor2B,LOW);
        }
    }
    
    _steeringServo.write(deg);
    
    if(d==0)
        delay(t);
        
    else if(t == 0){
        _encoderDist(d);
    }
    
    _stopMotion(200);
}

void AV4Wheel::_encoderDist(float d){
    _encoderPrevVal = LOW;
    int tickToTurn = ((d)/(WHEEL_CIRCUMFRENCE))*(TICKS_PER_ROTATION);
    int ticksTurned = 0;
    while(ticksTurned < tickToTurn){
        int n = digitalRead(_encoderPin);
        if ((_encoderPrevVal == LOW) && (n == HIGH)) {
            ticksTurned ++;
        } 
        _encoderPrevVal = n;
    }
}

void AV4Wheel::_stopMotion(int t){
    digitalWrite(_motor1A, LOW);
    digitalWrite(_motor1B, LOW);
    
    digitalWrite(_motor2A,LOW);
    digitalWrite(_motor2B,LOW);
    
    delay(t);
}