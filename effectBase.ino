#include "effectBase.hpp"
#include "helpers.hpp"

void Effect::Update(unsigned long time)
{
    prog += (int)((time - lastTime) * speed);
    prog = prog % duration;
    if (prog < 0)
        prog += duration;
    lastTime = time;
}

void Effect::Reset()
{
    prog = 0;
    progessOffset = 0;
#ifdef SIMULATOR
    lastTime = 0;
#else
    lastTime = millis();
#endif
}

int Effect::Progress()
{
    int p = prog;
    if (progessOffset)
    {
        p = (p + progessOffset) % duration;
        if (p < 0)
            p += duration;
    }
    return p;
}
