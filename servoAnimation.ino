#include "servoAnimation.hpp"

unsigned char SingleServoAnimation::Draw(unsigned long time)
{
    if (newState)
    {
        newState = false;
        lastStateChange = time;
    }

    unsigned long progress = time - lastStateChange;
    unsigned char value;

    if (state == LeftHold || state == RightHold)
    {
        value = (state == LeftHold) ? leftPoint : rightPoint;
    }
    else if (state == LeftTrans || state == RightTrans)
    {
        int beginExtent = (state == LeftTrans) ? rightPoint : leftPoint;
        int midPeak = (state == LeftTrans) ? leftPoint - 4 : rightPoint + 4;
        int endExtent = (state == LeftTrans) ? leftPoint : rightPoint;

        value = endExtent;
        if (progress < 2000)
            value = mapFloat(progress, 0, 2000, beginExtent, midPeak);
        else if (progress < 2500)
            value = mapFloat(progress, 2000, 2500, midPeak, endExtent);
        else
            SetState((state == LeftTrans) ? LeftHold : RightHold);
    }
    return value;
}

void SingleServoAnimation::SetState(enum State target)
{
    newState = true;
    state = target;
}

void DualServoAnimation::Transition()
{
    int i = rand() % 10;
    if (i == 0)
    {
        front.Right();
        back.Left();
    }
    else if (i == 1)
    {
        front.Left();
        back.Right();
    }
    else if (i >= 2 && i <= 5)
    {
        front.Right();
        back.Right();
    }
    else
    {
        front.Left();
        back.Left();
    }
}

void DualServoAnimation::Draw(unsigned long time)
{
    unsigned long progress = time - lastStateChange;

    val[0] = front.Draw(time);
    val[1] = back.Draw(time);

    if (progress > 10000)
    {
        lastStateChange = time;
        srand(time);
        Transition();
    }
}