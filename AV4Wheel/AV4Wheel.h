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
        void init(int m1a, int m1b, int m2a, int m2b, int ep, int sp, float wc);
        void init(int m1, int ep, int sp, float wc);
        void init(int ma, int mb, int ep, int sp, float wc);
        void setServo(int sd);
        void initUltra(uint8_t trigger_pin, uint8_t echo_pin, int max_cm_distance);
        void moveUltra(int d, boolean l, boolean i, int s, int deg);
        int getUltraIn();
        void diffMove(boolean i, int s, int deg);
        void stopMotion(int t);
    private:
        int _motor1A;
        int _motor1B;
        int _motor2A;
        int _motor2B;
        int _encoderPin;
        int _servoPin;
        int _mode;
        int _encoderPrevVal;
        int _triggerPin;
        int _echoPin;
        float _wheelCircumfrence;
        Servo _steeringServo;
        void _encoderDist(float d);
        int _ping_in();
        uint8_t _triggerBit;
        uint8_t _echoBit;
        volatile uint8_t *_triggerOutput;
        volatile uint8_t *_triggerMode;
        volatile uint8_t *_echoInput;
        unsigned int _maxEchoTime;
        unsigned long _max_time;
        boolean ping_trigger();
};

#endif