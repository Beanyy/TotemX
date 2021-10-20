#include "effects.hpp"
#include "helpers.hpp"

void EffectParticle::Draw(LedStrip *strip)
{
    int p = this->Progress();
    CHSV color = (this->forceColor) ? forceColorHsv : colorHsv;

    int offset = mapFloat(p, 0, this->duration, 0, strip->Size());
    strip->SetDir(reverse).SetOffset(offset).SetWrap(true).SetViewport(0, strip->Size());
    if (gradient)
        strip->DrawGradient(color, color.h + hueLength, streakSize);
    else
        strip->DrawStreak(color, streakSize, false);
}

void EffectParticle2::Draw(LedStrip *strip)
{
    int p = this->Progress();
    CHSV color = (this->forceColor) ? forceColorHsv : colorHsv;

    int offset = mapFloat(p, 0, this->duration, 0, strip->Size());
    strip->SetDir(reverse).SetOffset(offset).SetWrap(true).SetViewport(0, strip->Size());

    CHSV endColor = CHSV(color.h + offsetH, color.s + offsetS, color.v + offsetV);
    strip->DrawLerp(color, endColor, streakSize);
}

void EffectRainbow::Draw(LedStrip *strip)
{
    strip->SetDir(true).SetOffset(0).SetWrap(false).SetViewport(0, strip->Size());
    int hueOffset = mapFloat(this->Progress(), 0, this->duration, 0, 255);
    if (!mode)
        strip->DrawColor(CHSV(hueOffset, 255, 100));
    else
        strip->DrawRandom(60);
}

void EffectTheater::Draw(LedStrip *strip)
{
    const int skip = 3;
    CRGB color = (this->forceColor) ? this->forceColorRgb : this->colorRgb;
    int idx = (this->Progress() / 50) % skip;
    strip->SetDir(true).SetOffset(0).SetWrap(false).SetViewport(0, strip->Size());
    for (int i = idx; i < strip->Size(); i += skip)
        strip->SetLED(i, color);
}

void EffectFlash::Draw(LedStrip *strip)
{
    const int onDuration = 50;
    int remainder = strip->Size() % nSegments;
    // if (strip->Size() % nSegments)
    //     stripSize += 1;
    int segment = 0;
    int offset = mapFloat(this->Progress(), 0, this->duration, 0, this->duration / onDuration);
    fadeToBlackBy(strip->Leds(), strip->Size(), 10);

    strip->SetDir(true).SetWrap(false);
    srand(seed);
    for (int i = 0; i < nFlashes; i++) {
        int stripSize = strip->Size() / nSegments;
        for (int i = 0; i < offset; i++)
            segment = rand() % nSegments;

        int ledOffset = 0;
        for (size_t i = 0; i < segment; i++)
            ledOffset += (i < remainder) ? stripSize + 1 : stripSize;

        if (segment < remainder)
            stripSize += 1;

        strip->SetOffset(ledOffset).SetViewport(ledOffset, stripSize);
        strip->DrawColor(CHSV(offset * 144 + 256/nFlashes  * i, 255, 255));
    }
}

void EffectFill::Draw(LedStrip *strip)
{
    CHSV color = (this->forceColor) ? this->forceColorHsv : this->colorHsv;
    strip->SetViewport(0, strip->Size()).DrawColor(color);
}

void EffectWipe::Draw(LedStrip *strip)
{
    CHSV color = (this->forceColor) ? this->forceColorHsv : this->colorHsv;
    float offset = mapFloat(this->Progress(), 0, this->duration, 0, strip->Size());
    if (invert)
        strip->SetViewport(offset + ledOffset, strip->Size() - offset);
    else
        strip->SetViewport(ledOffset, offset);
    strip->SetDir(true).SetOffset(0).SetWrap(true).DrawColor(color);
}

void EffectBreathe::Draw(LedStrip *strip)
{
    CHSV color = (this->forceColor) ? this->forceColorHsv : this->colorHsv;
    int brightness = mapFloat(this->Progress(), 0, this->duration, 0, 511);
    if (brightness > 255)
        brightness = 511 - brightness;
    strip->SetDir(true).SetOffset(0).SetWrap(false).SetViewport(0, strip->Size()).DrawColor(CHSV(color.h, color.s, brightness));
}

void EffectServoLevel::Draw(DCMotor* motor)
{
    int sequencePos = min(mapFloat(this->Progress(), 0, this->duration, 0, mSequenceLen), mSequenceLen - 1);


    int remainingLevelTime = mTimePerLevel - this->Progress() % mTimePerLevel;
    float level;
    if (mTransitionTime >= remainingLevelTime) {
        int nextSequencePos = (sequencePos + 1) % mSequenceLen;
        level = mapFloat(mTransitionTime - remainingLevelTime, 0, mTransitionTime, mSequence[sequencePos], mSequence[nextSequencePos]);
    } else {
        level = mSequence[sequencePos];
    }

    motor->SetLevel(level);
}
