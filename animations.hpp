#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP
#include "animationBase.hpp"
#include "effects.hpp"

class AniWipe : public Animation
{
private:
    EffectWipe wipe;
    LedStrip *inner;
    LedStrip *outer;
    unsigned char hue = HUE_GREEN;
    int lastProgress = 0;

public:
    AniWipe(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                outer(outer)
    {
        this->AddEffect(&wipe);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniFlash : public Animation
{
private:
    EffectFlash flash;
    LedStrip *inner;
    LedStrip *outer;

public:
    AniFlash(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                 outer(outer)
    {
        this->AddEffect(&flash);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniParticle : public Animation
{
private:
    EffectParticle particle;
    Effect color;
    LedStrip *inner;
    LedStrip *outer;

public:
    AniParticle(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                    outer(outer)
    {
        color.duration = DEFAULT_BPM*10;
        this->AddEffect(&color);
        particle.duration = DEFAULT_BPM;
        this->AddEffect(&particle);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniConfetti : public Animation
{
private:
    LedStrip *leds;
    Effect hue;

public:
    AniConfetti(LedStrip *leds) : leds(leds)
    {
        hue.duration = DEFAULT_BPM*8;
        this->AddEffect(&hue);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniZoom : public Animation
{
private:
    EffectParticle particle;
    EffectFill fill;
    Effect color;
    LedStrip *inner;
    LedStrip *outer;

public:
    AniZoom(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                outer(outer)
    {
        color.duration = DEFAULT_BPM*15;
        particle.duration = DEFAULT_BPM;
        this->AddEffect(&color);
        this->AddEffect(&particle);
        this->AddEffect(&fill);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniSparkle : public Animation
{
private:
    EffectRainbow rainbow;
    LedStrip *leds;

public:
    AniSparkle(LedStrip *leds) : leds(leds)
    {
        this->AddEffect(&rainbow);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniRainbow : public Animation
{
private:
    EffectParticle particle;
    LedStrip *inner;
    LedStrip *outer;

public:
    AniRainbow(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                  outer(outer)
    {
        particle.duration = DEFAULT_BPM;
        this->AddEffect(&particle);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniBreathe : public Animation
{
private:
    EffectBreathe breathe;
    Effect color;
    LedStrip *leds;

public:
    AniBreathe(LedStrip *leds) : leds(leds)
    {
        breathe.duration = DEFAULT_BPM*3;
        color.duration = DEFAULT_BPM*30;
        this->AddEffect(&breathe);
        this->AddEffect(&color);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniFill : public Animation
{
private:
    EffectFill fill;
    Effect color;
    LedStrip *leds;

public:
    AniFill(LedStrip *leds) : leds(leds)
    {
        fill.duration = DEFAULT_BPM;
        color.duration = DEFAULT_BPM*30;
        this->AddEffect(&fill);
        this->AddEffect(&color);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniMultiParticle : public Animation
{
private:
    EffectParticle particle;
    Effect color;
    LedStrip *leds[2];

public:
    AniMultiParticle(LedStrip *inner, LedStrip *outer)
    {
        leds[0] = inner;
        leds[1] = outer;
        color.duration = DEFAULT_BPM*15;
        this->AddEffect(&color);
        particle.duration = DEFAULT_BPM;
        this->AddEffect(&particle);
    }
    virtual void DrawImpl(unsigned long time) override;
};

template <int N>
class AniFire : public Animation
{
private:
    LedStrip *inner;
    LedStrip *outer;
    uint8_t heat[N];
    Effect hueShift;
    uint8_t lastHueOffset;

public:
    AniFire(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                  outer(outer)
    {
        hueShift.duration = DEFAULT_BPM*8;
        this->AddEffect(&hueShift);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniWheel : public Animation
{
private:
    EffectParticle2 particle;
    EffectFill fill;
    Effect color;
    LedStrip *inner;
    LedStrip *outer;

public:
    AniWheel(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                outer(outer)
    {
        color.duration = DEFAULT_BPM*15;
        particle.duration = DEFAULT_BPM;
        this->AddEffect(&color);
        this->AddEffect(&particle);
        this->AddEffect(&fill);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniServo : public Animation
{
private:
    EffectServoLevel servoLevel;
    DCMotor *motor;

public:
    AniServo(DCMotor *motor) : motor(motor)
    {
        this->AddEffect(&servoLevel);
    }
    virtual void DrawImpl(unsigned long time) override;
};

#endif