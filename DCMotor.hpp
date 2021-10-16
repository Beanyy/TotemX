#ifndef DCMOTOR_HPP
#define DCMOTOR_HPP

#include "DCMotor.hpp"
#include "RotaryEncoder.h"

class DCMotor
{
private:
    int mPwmPin;
    int mForwardPin;
    int mReversePin;

    long prevTime = 0;
    float integral = 0;
    float prevError = 0;
    int lastPosition = 0;

    float kp = 2;
    float kd = 0.025;
    float ki = 0.0;
    void setMotor(int dir, int pwmVal)
    {
        analogWrite(mPwmPin,pwmVal);
        if(dir == 1){
            digitalWrite(mForwardPin,HIGH);
            digitalWrite(mReversePin,LOW);
        }
        else if(dir == -1){
            digitalWrite(mForwardPin,LOW);
            digitalWrite(mReversePin,HIGH);
        }
        else{
            digitalWrite(mForwardPin,LOW);
            digitalWrite(mReversePin,LOW);
        }  
    }
public:
    int position = 0;
    DCMotor(int pwmPin, int forwardPin, int reversePin) :
        mPwmPin(pwmPin),
        mForwardPin(forwardPin),
        mReversePin(reversePin)
    {
        pinMode(mPwmPin, OUTPUT);
        pinMode(mForwardPin, OUTPUT);
        pinMode(mReversePin, OUTPUT);
        digitalWrite (mForwardPin, 0);
        digitalWrite (mReversePin, 0);
    };

    void SetLevel(int speedLevel)
    {
        bool reverse = false;
        if (!speedLevel) {
            SetSpeed(0);
            return;
        }
        if (speedLevel < 0) {
            reverse = true;
            speedLevel = speedLevel * -1;
        }

        float targetSpeed = 40 + speedLevel * 35;
        targetSpeed = min(targetSpeed, 255);
        targetSpeed = (reverse) ? targetSpeed * -1 : targetSpeed;
        SetSpeed(targetSpeed);
    };
    void SetSpeed(float targetSpeed)
    {
        unsigned long now = millis();
        float deltaTime = ((float) (now - prevTime))/( 1.0e3 );
        prevTime = now;

        float speed = ((float)(position - lastPosition))/deltaTime/8;
        lastPosition = position;

        int error = targetSpeed - speed;
        float derivative = (error-prevError)/(deltaTime);
        integral = integral + error*deltaTime;

        // control signal
        float u = kp*error + kd*derivative + ki*integral;
        prevError = error;

        // motor power
        float pwr = fabs(u);
        if( pwr > 255 ){
            pwr = 255;
        }

        // motor direction
        int dir = 1;
        if(u<0){
            dir = -1;
        }

        setMotor(dir,pwr);
    };
};
#endif
