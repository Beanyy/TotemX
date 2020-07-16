#include "effects.hpp"
#include "helpers.hpp"

void AniParticle::Draw(LedStrip *strip)
{
    int p = this->Progress();

    int offset = mapFloat(p, 0, this->duration, 0, this->duration / 25);
    int hueOffset = colorCycle ? mapFloat(p, 0, this->duration, 0, 255) : 0;

    strip->SetDir(true).SetOffset(offset).SetWrap(true).SetViewport(0, strip->Size()).DrawStreak(streakSize, hue + hueOffset, false, 255);
}

void AniRainbow::Draw(LedStrip *strip)
{
    strip->SetDir(true).SetOffset(0).SetWrap(false).SetViewport(0, strip->Size());
    int hueOffset = mapFloat(this->Progress(), 0, this->duration, 0, 255);
    if (!mode)
        strip->DrawColor(CHSV(hueOffset, 255, 100));
    else
        strip->DrawRandom(3);
}

void AniTheater::Draw(LedStrip *strip)
{
    const int skip = 3;
    int idx = (this->Progress() / 50) % skip;
    strip->SetDir(true).SetOffset(0).SetWrap(false).SetViewport(0, strip->Size());
    for (int i = idx; i < strip->Size(); i += skip)
        strip->SetLED(i, this->color);
}

void AniFlash::Draw(LedStrip *strip)
{
    const int onDuration = 50;
    const int nSegments = 12;
    const int nFlashes = 2;

    int stripSize = strip->Size()/nSegments;
    int segment = 0;
    int offset = mapFloat(this->Progress(), 0, this->duration, 0, this->duration / onDuration);
    fadeToBlackBy(strip->Leds(), strip->Size(), 10);

    strip->SetDir(true).SetWrap(false);
    srand(10);
    for (int i = 0; i < nFlashes; i++) {
        for (int i = 0; i < offset; i++)
            segment = rand() % nSegments;
        
        int ledOffset = segment * stripSize;
        strip->SetOffset(ledOffset).SetViewport(ledOffset, stripSize);
        strip->DrawColor(CHSV(offset * 144 + 256/nFlashes  * i, 255, 100));
    }
}

void AniWipe::Draw(LedStrip *strip)
{
    float offset = mapFloat(this->Progress(), 0, this->duration, 0, strip->Size());
    if (invert)
        strip->SetViewport(offset, strip->Size() - offset);
    else
        strip->SetViewport(0, offset);
    strip->SetDir(true).SetOffset(0).SetWrap(false).DrawColor(this->color);
}