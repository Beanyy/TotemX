#ifndef EFFECTS_HPP
#define EFFECTS_HPP
#include "effectBase.hpp"
#include "DCMotor.hpp"
#define BPM128 1875

class EffectServo : public Effect
{
public:
	virtual void Draw(DCMotor* motor) = 0;
};

class EffectServoSine : public EffectServo
{
public:
	EffectServoSine() {
		this->duration = BPM128*8;
	}
	virtual void Draw(DCMotor* motor) override;
};

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
		this->duration = BPM128;
		reverse = false;
	}
	virtual void Draw(LedStrip* strip) override;
};

class EffectTheater : public EffectStrip
{
public:
	EffectTheater() {
		this->duration = BPM128;
	}
	virtual void Draw(LedStrip* strip) override;
};

class EffectWipe : public EffectStrip
{
public:
	EffectWipe() {
		this->duration = BPM128;
	}
	char invert;
	int ledOffset;
	virtual void Draw(LedStrip* strip) override;
};

class EffectRainbow : public EffectStrip
{
public:
	EffectRainbow() {
		this->duration = BPM128;
	}
	int mode;
	virtual void Draw(LedStrip* strip) override;
};

class EffectFlash : public EffectStrip
{
public:
  EffectFlash() {
    this->duration = BPM128*4;
  }
  int seed;
  virtual void Draw(LedStrip* strip) override;
};

class EffectBreathe : public EffectStrip
{
public:
  EffectBreathe() {
    this->duration = BPM128;
  }
  unsigned char hue;
  virtual void Draw(LedStrip* strip) override;
};
#endif
