#ifndef SERVOANIMATION_HPP
#define SERVOANIMATION_HPP

#include "helpers.hpp"

class SingleServoAnimation
{
private:
    enum State
    {
        LeftHold,
        RightHold,
        LeftTrans,
        RightTrans,
    } state;
    void SetState(enum State target);
    int leftPoint;
    int rightPoint;
    unsigned long lastStateChange;
    bool newState;

public:
    SingleServoAnimation(int leftPoint, int rightPoint) : leftPoint(leftPoint),
                                                          rightPoint(rightPoint),
                                                          state(LeftHold),
                                                          newState(true)
    {
    }

    unsigned char Draw(unsigned long time);
    void Left()
    {
        if (state != LeftHold)
            SetState(LeftTrans);
    }
    void Right()
    {
        if (state != RightHold)
            SetState(RightTrans);
    }
};
class DualServoAnimation
{
private:
    enum State
    {
        Left,
        Right,
        CounterLeft,
        CounterRight
    } state;
    void Transition();
    SingleServoAnimation front;
    SingleServoAnimation back;
    unsigned long lastStateChange;

public:
    DualServoAnimation(int leftFront, int rightFront, int leftBack, int rightBack) : front{leftFront, rightFront},
                                                                                     back{leftBack, rightBack}
    {
        lastStateChange = 0;
    }
    void Draw(unsigned long time);
    unsigned char val[2];
};
#endif
