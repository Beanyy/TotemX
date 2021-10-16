#include "helpers.hpp"
#include <FastLED.h>

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

CHSV RgbToHsv(CRGB rgb)
{
    CHSV hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}
