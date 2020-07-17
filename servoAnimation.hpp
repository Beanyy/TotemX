#ifndef SERVOANIMATION_HPP
#define SERVOANIMATION_HPP

class ServoAnimation {
private:
    enum State {
        Zero,
        LeftHold,
        RightHold,
        LeftTrans,
        RightTrans
    } state;
    unsigned char leftPoint;
    unsigned char zeroPoint;
    unsigned char rightPoint;
    unsigned long lastStateChange;

    enum State getNextState()
    {
        enum State targetState = (rand() % 3) ? LeftHold : RightHold;
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

public:
    ServoAnimation(unsigned char leftPoint, unsigned char rightPoint, unsigned char zeroPoint):
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
            unsigned char endExtent   = (state == LeftTrans) ? leftPoint  : rightPoint;
                     char overshoot   = (state == LeftTrans) ? -4         : 4;
            
            if (progress < 4000) {
                value = mapFloat(progress, 0, 4000, beginExtent, endExtent + overshoot);
            } else if (progress < 4500) {
                value = mapFloat(progress, 4000, 4500, endExtent + overshoot, endExtent);
            } else {
                stateChange = true;
                value = endExtent;
            }
        }

        if (stateChange)
            nextState = getNextState();

        if (state != nextState) {
            lastStateChange = time;
            if (state == Zero)
                lastStateChange -= 2500;
            state = nextState;
        }
    }
};

#endif