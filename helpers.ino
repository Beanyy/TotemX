#include "helpers.hpp"

#ifdef SIMULATOR
template <class T>
T constrain(T in, T lower, T upper)
{
    if (in < lower)
        return lower;
    else if (in > upper)
        return upper;
    else
        return in;
}

template float constrain<float>(float in, float lower, float upper);

uint16_t lerp16by8(uint16_t from, uint16_t to, uint8_t frac)
{
    float diff = to - from;
    float fracDiff = diff * frac / 255;
    return (uint16_t)((float)from + fracDiff);
}
#endif
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
