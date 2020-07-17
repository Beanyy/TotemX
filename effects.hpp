#ifndef EFFECTS_HPP
#define EFFECTS_HPP
#include "effectBase.hpp"

class EffectStrip : public Effect
{
public:
	virtual void Draw(LedStrip* strip) = 0;
};

class EffectParticle : public EffectStrip
{
public:
	int streakSize;
	unsigned char hue;
	bool colorCycle;
	EffectParticle() {
		this->duration = 80000;
	}
	virtual void Draw(LedStrip* strip) override;
};

class EffectTheater : public EffectStrip
{
public:
	EffectTheater() {
		this->duration = 1050;
	}
	virtual void Draw(LedStrip* strip) override;
};

class EffectWipe : public EffectStrip
{
public:
	EffectWipe() {
		this->duration = 5000;
	}
	char invert;
	virtual void Draw(LedStrip* strip) override;
};

class EffectRainbow : public EffectStrip
{
public:
	EffectRainbow() {
		this->duration = 1000;
	}
	int mode;
	virtual void Draw(LedStrip* strip) override;
};

class EffectFlash : public EffectStrip
{
public:
  EffectFlash() {
    this->duration = 10000;
	seed = 10;
  }
  int seed;
  virtual void Draw(LedStrip* strip) override;
};
#endif
