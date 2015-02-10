/*
  AV3Wheel-3Wheel Drive library for AIVD
*/

#ifndef AV3Wheel_h
#define AV3Wheel_h

#include "Arduino.h"

class AV3Wheel{
    public:
        AV3Wheel();
        void forward(boolean i, int s, float d, int t);
        void spin(boolean cw, int s, float d, int t);
        void circle(boolean cw, int s1, int s2, float d, int t);
        void init(int m1a, int m1b, int m2a, int m2b, int ep, float wc);
    private:
        int _motor1A;
        int _motor1B;
        int _motor2A;
        int _motor2B;
        int _encoderPin;
        int _encoderPrevVal;
        float _wheelCircumfrence;
        void _encoderDist(float d);
        void _stopMotion(int t);
};

#endif