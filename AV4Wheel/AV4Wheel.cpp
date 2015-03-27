/*
  AV4Wheel-4Wheel Drive library for AIVD
*/

#include "Arduino.h"
#include "AV4Wheel.h"

#define TICKS_PER_ROTATION 90

AV4Wheel::AV4Wheel(){}

void AV4Wheel::init(int m1a, int m1b, int m2a, int m2b, int ep, int sp, float wc){
    _motor1A = m1a;
    _motor1B = m1b;
    _motor2A = m2a;
    _motor2B = m2b;
    _encoderPin = ep;
    _servoPin = sp;
    
    _mode = 1;
    
    _encoderPrevVal = LOW;
    
    _wheelCircumfrence = wc;
    
    pinMode(_motor1A,OUTPUT);
    pinMode(_motor1B,OUTPUT);
    pinMode(_motor2A,OUTPUT);
    pinMode(_motor2B,OUTPUT);
    
    pinMode(_encoderPin,INPUT);
    
    _steeringServo.attach(_servoPin);
}

void AV4Wheel::init(int ma, int mb, int ep, int sp, float wc){
    _motor1A = ma;
    _motor1B = mb;
    _encoderPin = ep;
    _servoPin = sp;
    
    _mode = 2;
    
    _encoderPrevVal = LOW;
    
    _wheelCircumfrence = wc;
    
    pinMode(_motor1A,OUTPUT);
    pinMode(_motor1B,OUTPUT);
    
    pinMode(_encoderPin,INPUT);
    
    _steeringServo.attach(_servoPin);
}

void AV4Wheel::init(int m1, int ep, int sp, float wc){
    _motor1A = m1;
    _encoderPin = ep;
    _servoPin = sp;
    
    _mode = 3;
    
    _encoderPrevVal = LOW;
    
    _wheelCircumfrence = wc;
    
    pinMode(_motor1A,OUTPUT);
    
    pinMode(_encoderPin,INPUT);
    
    _steeringServo.attach(_servoPin);
}

void AV4Wheel::move(boolean i, int s, int deg, float d, int t){
    if(i){
        if(_mode == 2){
            digitalWrite(_motor1A, LOW);
            digitalWrite(_motor1B, s);
        }
        else if(_mode == 1){
            digitalWrite(_motor1A, LOW);
            analogWrite(_motor1B, s);
            
            digitalWrite(_motor2A,LOW);
            analogWrite(_motor2B,s);
        }
        else{
            analogWrite(_motor1A, s);
        }
    }
    else{
        if(_mode == 2){
            analogWrite(_motor1A, s);
            digitalWrite(_motor1B, LOW);
        }
        else if(_mode == 1){
            analogWrite(_motor1A, s);
            digitalWrite(_motor1B, LOW);
            
            analogWrite(_motor2A,s);
            digitalWrite(_motor2B,LOW);
        }
        else{
            analogWrite(_motor1A, s);
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
    int tickToTurn = ((d)/(_wheelCircumfrence))*(TICKS_PER_ROTATION);
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