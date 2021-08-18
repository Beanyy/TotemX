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
    float ki = 0.1;
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
        digitalWrite (PB5, 0);
        digitalWrite (PB4, 0);
    };

    void SetSpeed(float targetSpeed)
    {
        unsigned long now = millis();
        float deltaTime = ((float) (now - prevTime))/( 1.0e3 );
        prevTime = now;

        float speed = ((float)(position - lastPosition))/deltaTime;
        lastPosition = position;
        Serial.print(speed);
        Serial.print(" ");

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

        Serial.print(targetSpeed);
        Serial.print(" ");
        Serial.print(pwr);
        Serial.print(" ");
        Serial.println(dir);

        setMotor(dir,pwr);
    };
};
#endif