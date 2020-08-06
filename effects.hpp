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
	unsigned char hueBegin;
	bool reverse;
	bool gradient;
	EffectParticle() {
		this->duration = 2000;
		reverse = false;
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
		this->duration = 1000;
	}
	char invert;
	int ledOffset;
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
  }
  int seed;
  virtual void Draw(LedStrip* strip) override;
};

class EffectBreathe : public EffectStrip
{
public:
  EffectBreathe() {
    this->duration = 2000;
  }
  unsigned char hue;
  virtual void Draw(LedStrip* strip) override;
};
#endif
