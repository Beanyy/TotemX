#include "LedStrip.hpp"
#include "helpers.hpp"
#include <FastLED.h>
#ifdef SIMULATOR
#include <stdlib.h>
#endif

static bool InRange(int in, int lower, int upper)
{
    return (in >= lower && in < upper);
}

LedStrip::LedStrip(CRGB *leds_, int nLeds_) : leds(leds_),
                                              nLeds(nLeds_)
{
    Reset();
    Clear();
}

int LedStrip::CalcViewport(int pos)
{
    int frac = 0.0f;
    pos *= 256;
    int vpSize = viewport[1] - viewport[0];
    int fracL = viewport[0] - pos;
    int fracR = viewport[1] - pos;
    if (fracL <= 0 && fracR >= 256)
    {
        if (vpSize < 256)
            return vpSize;
        return 256;
    }
    if (fracL > 0 && fracL < 256)
        frac += min(vpSize, 256 - fracL);
    if (fracR > 0 && fracR < 256)
        frac += min(vpSize, fracR);

    int fracL2 = fracL - nLeds * 256;
    int fracR2 = fracR - nLeds * 256;
    if (fracL2 <= 0 && fracR2 >= 256)
        return 256;
    else if (fracR2 > 0 && fracR2 < 256)
        frac += fracR2;

    return frac;
}

LedStrip &LedStrip::SetViewport(float pos, float size)
{
    float start, end;

    size = constrain(size, -nLeds, nLeds);
    start = pos;
    end = pos + size;

    viewport[0] = (min(start, end)) * 256;
    viewport[1] = (max(start, end)) * 256;
    if (viewport[0] < 0)
    {
        viewport[0] += nLeds;
        viewport[1] += nLeds;
    }
    return *this;
}

LedStrip &LedStrip::SetOffset(int o)
{
    offset = o;
    return *this;
}

LedStrip &LedStrip::SetWrap(bool w)
{
    wrap = w;
    return *this;
}

LedStrip &LedStrip::SetDir(bool f)
{
    forward = f;
    return *this;
}

LedStrip &LedStrip::SetBlendMode(BlendMode mode)
{
    blendMode = mode;
    return *this;
}

LedStrip &LedStrip::SetLED(int pos, CRGB color)
{
    int element = pos + offset;
    if (wrap)
    {
        element = element % nLeds;
        if (element < 0)
            element += nLeds;
    }

    if (!InRange(element, 0, nLeds))
        return *this;

    int frac = CalcViewport(element);

    if (!forward)
        element = nLeds - element - 1;

    if (frac == 256)
    {
        if (blendMode == BLEND_MODE_NONE) {
          leds[element] = color;
        } else if (blendMode == BLEND_MODE_HALF) {
          leds[element].r = (leds[element].r * 128 + color.r * 128) / 256;
          leds[element].g = (leds[element].g * 128 + color.g * 128) / 256;
          leds[element].b = (leds[element].b * 128 + color.b * 128) / 256;
        } else if (blendMode == BLEND_MODE_ADD) {
          leds[element].r = min(leds[element].r + color.r, 255);
          leds[element].g = min(leds[element].g + color.g, 255);
          leds[element].b = min(leds[element].b + color.b, 255);
        } else if (blendMode == BLEND_MODE_MAXIMUM) {
          leds[element].r = max(color.r, leds[element].r);
          leds[element].g = max(color.g, leds[element].g);
          leds[element].b = max(color.b, leds[element].b);
        }
    }
    else if (frac > 0)
    {
        leds[element].r = (leds[element].r * (256 - frac) + color.r * frac) / 256;
        leds[element].g = (leds[element].g * (256 - frac) + color.g * frac) / 256;
        leds[element].b = (leds[element].b * (256 - frac) + color.b * frac) / 256;
    }
    return *this;
}

LedStrip &LedStrip::Reset()
{
    SetViewport(0, nLeds);
    SetOffset(0);
    SetWrap(false);
    SetDir(true);
    // for (int i = 0; i < nLeds; i++)
    //     leds[i] = CRGB::Black;
    return *this;
}

LedStrip &LedStrip::Clear()
{
    DrawColor(CRGB::Black);
    return *this;
}

LedStrip &LedStrip::DrawColor(CRGB color)
{
    for (int i = 0; i < nLeds; i++)
        SetLED(i, color);
    return *this;
}

LedStrip &LedStrip::DrawGradient(CHSV color, uint16_t hueEnd, int size)
{
    if (color.h > hueEnd)
        hueEnd += 255;
        
    for (int i = 0; i < size; i++)
    {
        uint8_t actualHue = lerp16by8(color.h, hueEnd, 255 * i / (size - 1));
        SetLED(i, CHSV(actualHue, color.s, color.v));
    }
    return *this;
}

LedStrip &LedStrip::DrawLerp(CRGB colorStart, CRGB colorEnd, int size)
{
    for (int i = 0; i < size; i++)
    {
        uint8_t frac = lerp8by8(0, 255, 255 * i / (size - 1));
        uint8_t r = (colorStart.r * (255 - frac) + colorEnd.r * frac) / 255;
        uint8_t g = (colorStart.g * (255 - frac) + colorEnd.g * frac) / 255;
        uint8_t b = (colorStart.b * (255 - frac) + colorEnd.b * frac) / 255;
        SetLED(i, CRGB(r,g,b));
    }
    return *this;
}

LedStrip &LedStrip::DrawRandom(int num)
{
    for (uint8_t i = 0; i < num; i++)
    {
        uint8_t pos = rand() % nLeds;
        SetLED(pos, CHSV(rand(), 255, 255));
    }
    return *this;
}

LedStrip &LedStrip::DrawTriangle(int size, uint8_t hue)
{
    int odd = size % 2;
    int halfSize = size / 2 + odd;
    for (int i = 0; i < halfSize; i++)
    {
        CRGB color = CHSV(hue, 255, 255 * (halfSize - i) / halfSize);
        SetLED(i, color);
        SetLED(-i - 1 + odd, color);
    }
    return *this;
}

LedStrip &LedStrip::DrawStreak(CHSV color, int size, bool fowards)
{
    for (int i = 0; i < size; i++)
    {
        CRGB actualColor = CHSV(color.h, color.s, color.v * (size - i) / size);
        SetLED(fowards ? i : -i, actualColor);
    }
    return *this;
}


static void Swap(CRGB *a, CRGB *b)
{
    CRGB temp = *a;
    *a = *b;
    *b = temp;
}

static void ReverseArray(CRGB *leds, int startIndex, int endIndex)
{
    int i;
    CRGB *start = &leds[startIndex];
    int size = endIndex - startIndex + 1;
    for (i = 0; i < size / 2; i++)
        Swap(&start[i], &start[size - 1 - i]);
}

LedStrip &LedStrip::Shift(int m)
{
    ReverseArray(leds, 0, nLeds - 1);
    ReverseArray(leds, 0, m - 1);
    ReverseArray(leds, m, nLeds - 1);
    return *this;
}
