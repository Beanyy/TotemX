#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP
#include "animationBase.hpp"
#include "effects.hpp"

class AniWipe : public Animation {
private:
    EffectWipe wipe;
    LedStrip* inner;
    LedStrip* outer;
    unsigned char hue = HUE_GREEN;
    int lastProgress = 0;

public:
    AniWipe(LedStrip *inner, LedStrip *outer):
        inner(inner),
        outer(outer) 
    {
        this->AddEffect(&wipe);
    }
	virtual void DrawImpl(unsigned long time) override;
};

#endif