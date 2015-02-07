/*
  AV4Wheel-4Wheel Drive library for AIVD
*/

#ifndef AV4Wheel_h
#define AV4Wheel_h

#include "Arduino.h"
#include "Servo.h"

class AV4Wheel{
    public:
        AV4Wheel();
        void move(boolean i, int s, int deg, float d, int t);
        void init(int m1a, int m1b, int m2a, int m2b, int ep, int sp);
        void init(int ma, int mb, int ep, int sp);
    private:
        int _motor1A;
        int _motor1B;
        int _motor2A;
        int _motor2B;
        int _encoderPin;
        int _servoPin;
        int _hasDifferential;
        int _encoderPrevVal;
        Servo _steeringServo;
        void _encoderDist(float d);
        void _stopMotion(int t);
};

#endif