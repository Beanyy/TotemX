#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP
#include "animationBase.hpp"
#include "effects.hpp"

class AniWipe : public Animation
{
private:
    EffectWipe wipe;
    Effect spin;
    LedStrip *inner;
    LedStrip *outer;
    unsigned char hue = HUE_GREEN;
    int lastProgress = 0;

public:
    AniWipe(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                outer(outer)
    {
        spin.duration = 3000;
        this->AddEffect(&spin);
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
        color.duration = 15000;
        this->AddEffect(&color);
        particle.duration = 1500;
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
        hue.duration = 15000;
        this->AddEffect(&hue);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniZoom : public Animation
{
private:
    EffectParticle particle;
    EffectBreathe breathe;
    Effect color;
    LedStrip *inner;
    LedStrip *outer;

public:
    AniZoom(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                outer(outer)
    {
        color.duration = 15000;
        particle.duration = 1000;
        breathe.duration = 1000;
        this->AddEffect(&color);
        this->AddEffect(&particle);
        this->AddEffect(&breathe);
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
        particle.duration = 1500;
        this->AddEffect(&particle);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniServo : public Animation
{
private:
    EffectServoSine sineWave;
    DCMotor *motor;

public:
    AniServo(DCMotor *motor) : motor(motor)
    {
        this->AddEffect(&sineWave);
    }
    virtual void DrawImpl(unsigned long time) override;
};

#endif