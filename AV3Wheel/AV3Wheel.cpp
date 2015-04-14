/*
  AV3Wheel-3Wheel Drive library for AIVD
*/

#include "Arduino.h"
#include "AV3Wheel.h"

#define TICKS_PER_ROTATION 90


AV3Wheel::AV3Wheel(){
}

void AV3Wheel::init(int m1a, int m1b, int m2a, int m2b, int ep, float wc){
    _motor1A = m1a;
    _motor1B = m1b;
    _motor2A = m2a;
    _motor2B = m2b;
    _encoderPin = ep;

    _encoderPrevVal = LOW;
    
    _wheelCircumfrence = wc;
    
    pinMode(_motor1A,OUTPUT);
    pinMode(_motor1B,OUTPUT);
    pinMode(_motor2A,OUTPUT);
    pinMode(_motor2B,OUTPUT);
    
    pinMode(_encoderPin,INPUT);
}

void AV3Wheel::forward(boolean i, int s, float d, int t){
    if(i){
        digitalWrite(_motor1A, LOW);
        analogWrite(_motor1B, s);
        
        digitalWrite(_motor2A,LOW);
        analogWrite(_motor2B,s);
    }
    else{
        analogWrite(_motor1A, s);
        digitalWrite(_motor1B, LOW);
        
        analogWrite(_motor2A,s);
        digitalWrite(_motor2B,LOW);   
    }
    
    if(d==0)
        delay(t);
        
    else if(t == 0){
        _encoderDist(d);
    }
    
    _stopMotion(200);
}

void AV3Wheel::spin(boolean cw, int s, float d, int t){
    if(cw){
        analogWrite(_motor1A, s);
        digitalWrite(_motor1B, LOW);
        
        digitalWrite(_motor2A,LOW);
        analogWrite(_motor2B,s);
    }
    else{
        digitalWrite(_motor1A, LOW);
        analogWrite(_motor1B, s);
        
        analogWrite(_motor2A,s);
        digitalWrite(_motor2B,LOW);   
    }
    
    if(d==0)
        delay(t);
    else if(t == 0){
        _encoderDist(d);
    }
    
    _stopMotion(200);
}

void AV3Wheel::circle(boolean i, int s1, int s2, float d, int t){
    if(i){
        analogWrite(_motor1A, s1);
        digitalWrite(_motor1B, LOW);
        
        analogWrite(_motor2A,s2);
        digitalWrite(_motor2B,LOW);
    }
    else{
        digitalWrite(_motor1A, LOW);
        analogWrite(_motor1B, s1);
        
        digitalWrite(_motor2A,LOW);
        analogWrite(_motor2B,s2);   
    }
        
    if(d==0)
        delay(t);
        
    else if(t == 0){
        _encoderDist(d);
    }
    
    _stopMotion(200);
}

void AV3Wheel::_encoderDist(float d){
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

void AV3Wheel::_stopMotion(int t){
    digitalWrite(_motor1A, LOW);
    digitalWrite(_motor1B, LOW);
    
    digitalWrite(_motor2A,LOW);
    digitalWrite(_motor2B,LOW);
    
    delay(t);
}