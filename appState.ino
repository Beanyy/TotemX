#include <string.h>
#include "appState.hpp"

AppState::AppState()
{
    modifiers[0] = {'0', sizeof(color.h), &color.h};
    modifiers[1] = {'1', sizeof(color.s), &color.s};
    modifiers[2] = {'2', sizeof(color.v), &color.v};
    modifiers[3] = {'a', sizeof(animation), &animation};
    modifiers[4] = {'s', sizeof(speed), &speed};
    modifiers[5] = {'m', sizeof(motionOverride), &motionOverride};
    modifiers[6] = {'c', sizeof(colorOverride), &colorOverride};
    modifiers[7] = {'o', sizeof(off), &off};
    modifiers[8] = {'p', sizeof(pause), &pause};
    Reset();
}

void AppState::Reset()
{
    color.h = 93;
    color.s = 255;
    color.v = 20;
    animation = -1;
    speed = 85;
    motionOverride = 0;
    colorOverride = 0;
    off = 0;
    pause = 0;
    return;
}
void AppState::Update(char *cmd)
{
    int value = strtol(&cmd[1], NULL, 10);
    for (int i = 0; i < nModifiers; i++)
    {
        if (cmd[0] == modifiers[i].cmd)
        {
            memcpy(modifiers[i].value, &value, modifiers[i].size);
            return;
        }
    }
}