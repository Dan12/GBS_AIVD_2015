/*
  AV4Wheel-4Wheel Drive library for AIVD
*/

#include "Arduino.h"
#include "AV4Wheel.h"

#define TICKS_PER_ROTATION 90

// Shoudln't need to changed these values unless you have a specific need to do so.
#define MAX_SENSOR_DISTANCE 500 // Maximum sensor distance can be as high as 500cm, no reason to wait for ping longer than sound takes to travel this distance and back.
#define US_ROUNDTRIP_IN 146     // Microseconds (uS) it takes sound to travel round-trip 1 inch (2 inches total), uses integer to save compiled code space.
#define US_ROUNDTRIP_CM 57      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space.
#define DISABLE_ONE_PIN false   // Set to "true" to save up to 26 bytes of compiled code space if you're not using one pin sensor connections.

// Probably shoudln't change these values unless you really know what you're doing.
#define NO_ECHO 0               // Value returned if there's no ping echo within the specified MAX_SENSOR_DISTANCE or max_cm_distance.
#define MAX_SENSOR_DELAY 18000  // Maximum uS it takes for sensor to start the ping (SRF06 is the highest measured, just under 18ms).
#define ECHO_TIMER_FREQ 24      // Frequency to check for a ping echo (every 24uS is about 0.4cm accuracy).
#define PING_MEDIAN_DELAY 29    // Millisecond delay between pings in the ping_median method.

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

void AV4Wheel::setServo(int sd){
    _steeringServo.write(sd);
}

void AV4Wheel::initUltra(uint8_t trigger_pin, uint8_t echo_pin, int max_cm_distance) {
    _triggerBit = digitalPinToBitMask(trigger_pin); // Get the port register bitmask for the trigger pin.
    _echoBit = digitalPinToBitMask(echo_pin);       // Get the port register bitmask for the echo pin.

    _triggerOutput = portOutputRegister(digitalPinToPort(trigger_pin)); // Get the output port register for the trigger pin.
    _echoInput = portInputRegister(digitalPinToPort(echo_pin));         // Get the input port register for the echo pin.

    _triggerMode = (uint8_t *) portModeRegister(digitalPinToPort(trigger_pin)); // Get the port mode register for the trigger pin.

    _maxEchoTime = min(max_cm_distance, MAX_SENSOR_DISTANCE) * US_ROUNDTRIP_CM + (US_ROUNDTRIP_CM / 2); // Calculate the maximum distance in uS.

    #if DISABLE_ONE_PIN == true
        *_triggerMode |= _triggerBit; // Set trigger pin to output.
    #endif
}

void AV4Wheel::moveUltra(int d, boolean l, boolean i, int s, int deg){
    setServo(deg);
    
    delay(200);
    
    diffMove(i,s);
    
    int dist = _ping_in();
    delay(29);

    if(l)
        while(dist < d && dist != 0){
            dist = _ping_in();
            delay(29);
        }
    else
        while(dist > d || dist == 0){
            dist = _ping_in();
            delay(29);
        }

    stopMotion(500);
}
int AV4Wheel::getUltraIn(){
    return _ping_in();
}

void AV4Wheel::move(boolean i, int s, int deg, float d, int t){
    setServo(deg);
    
    delay(200);
    
    diffMove(i,s);
    
    if(d==0)
        delay(t);
        
    else if(t == 0){
        _encoderDist(d);
    }
    
    stopMotion(500);
}

void AV4Wheel::diffMove(boolean i, int s){
    if(i){
        if(_mode == 2){
            digitalWrite(_motor1A, LOW);
            analogWrite(_motor1B, s);
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
            digitalWrite(_motor1A, HIGH);
            analogWrite(_motor1B, s);
        }
        else if(_mode == 1){
            digitalWrite(_motor1A, HIGH);
            analogWrite(_motor1B, s);
            
            digitalWrite(_motor2A,HIGH);
            analogWrite(_motor2B,s);
        }
        else{
            analogWrite(_motor1A, s);
        }
    }
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

void AV4Wheel::stopMotion(int t){
    if(_mode == 1 || _mode == 2){
        digitalWrite(_motor1A, LOW);
        digitalWrite(_motor1B, LOW);
    }
    
    if(_mode == 1){
        digitalWrite(_motor2A,LOW);
        digitalWrite(_motor2B,LOW);
    }
    
    if(_mode == 3){
        analogWrite(_motor1A, 10);
    }
    
    delay(t);
}

int AV4Wheel::_ping_in(){
    if (!ping_trigger()) return NO_ECHO;                // Trigger a ping, if it returns false, return NO_ECHO to the calling function.
    while (*_echoInput & _echoBit)                      // Wait for the ping echo.
        if (micros() > _max_time) return NO_ECHO;       // Stop the loop and return NO_ECHO (false) if we're beyond the set maximum distance.
    int uS = (micros() - (_max_time - _maxEchoTime) - 5); // Calculate ping time, 5uS of overhead.
    return uS / US_ROUNDTRIP_IN; 
}

boolean AV4Wheel::ping_trigger() {
#if DISABLE_ONE_PIN != true
    *_triggerMode |= _triggerBit;    // Set trigger pin to output.
#endif
    *_triggerOutput &= ~_triggerBit; // Set the trigger pin low, should already be low, but this will make sure it is.
    delayMicroseconds(4);            // Wait for pin to go low, testing shows it needs 4uS to work every time.
    *_triggerOutput |= _triggerBit;  // Set trigger pin high, this tells the sensor to send out a ping.
    delayMicroseconds(10);           // Wait long enough for the sensor to realize the trigger pin is high. Sensor specs say to wait 10uS.
    *_triggerOutput &= ~_triggerBit; // Set trigger pin back to low.
#if DISABLE_ONE_PIN != true
    *_triggerMode &= ~_triggerBit;   // Set trigger pin to input (when using one Arduino pin this is technically setting the echo pin to input as both are tied to the same Arduino pin).
#endif

    _max_time =  micros() + MAX_SENSOR_DELAY;                  // Set a timeout for the ping to trigger.
    while (*_echoInput & _echoBit && micros() <= _max_time) {} // Wait for echo pin to clear.
    while (!(*_echoInput & _echoBit))                          // Wait for ping to start.
        if (micros() > _max_time) return false;                // Something went wrong, abort.

    _max_time = micros() + _maxEchoTime; // Ping started, set the timeout.
    return true;                         // Ping started successfully.
}