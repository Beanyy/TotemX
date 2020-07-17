#ifndef SERVOANIMATION_HPP
#define SERVOANIMATION_HPP

#include "helpers.hpp"

class ServoAnimation {
private:

    unsigned char leftPoint;
    unsigned char zeroPoint;
    unsigned char rightPoint;
    



public:
    enum State {
        Zero,
        LeftHold,
        RightHold,
        LeftTrans,
        RightTrans
    } state;
       enum State getNextState()
    {
        enum State targetState = (rand() % 5) ? LeftHold : RightHold;
        switch(state) {
        case Zero:
            return (targetState == LeftHold) ? LeftTrans : RightTrans;
        case RightTrans:
            return RightHold;
        case LeftTrans:
            return LeftHold;
        case RightHold:
            return (targetState == LeftHold) ? LeftTrans : RightHold;
        case LeftHold:
            return (targetState == LeftHold) ? LeftHold : RightTrans;
        }
    }
    unsigned long lastStateChange;
    ServoAnimation(unsigned char leftPoint, unsigned char zeroPoint, unsigned char rightPoint):
        leftPoint(leftPoint),
        rightPoint(rightPoint),
        zeroPoint(zeroPoint)
    {
        state = Zero;
    }

	unsigned char Draw(unsigned long time)
    {
        enum State nextState = state;
        unsigned long progress = time - lastStateChange;
        unsigned char value = zeroPoint;
        bool stateChange = false;

        if (state == Zero) {
            stateChange = true;
        } else if (state == LeftHold || state == RightHold) {
            value = (state == LeftHold) ? leftPoint : rightPoint;
            if (progress > 7000)
                stateChange = true;
        } else if (state == LeftTrans || state == RightTrans) {
            unsigned char beginExtent = (state == LeftTrans) ? rightPoint : leftPoint;
            unsigned char midPeak     = (state == LeftTrans) ? leftPoint-4: rightPoint+4;
            unsigned char endExtent   = (state == LeftTrans) ? leftPoint  : rightPoint;
            
            if (progress < 2000) {
                value = mapFloat(progress, 0, 2000, beginExtent, midPeak);
            } else if (progress < 2500) {
                value = mapFloat(progress, 2000, 2500, midPeak, endExtent);
            } else {
                stateChange = true;
                value = endExtent;
            }
        }

        if (stateChange) {
            lastStateChange = time;
            if (state == Zero)
                lastStateChange -= 2500;
            state = getNextState();
        }
        return value;
    }
};

#endif
