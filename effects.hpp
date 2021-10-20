#ifndef EFFECTS_HPP
#define EFFECTS_HPP
#include "effectBase.hpp"
#include "DCMotor.hpp"

#define BPM128 1875
#define DEFAULT_BPM BPM128

class EffectServo : public Effect
{
public:
	virtual void Draw(DCMotor* motor) = 0;
};

class EffectServoLevel : public EffectServo
{
	const int mSequence[17] = {-1, 1, 1,-2,
								1, 1, 2,-1,
								2,-2, 1, 1,
								1,-1,-2, 2,
								2};
	const int mSequenceLen = sizeof(mSequence)/sizeof(mSequence[0]);
	const int mTransitionTime = BPM128/2;
	const int mTimePerLevel = BPM128*8;

public:
	EffectServoLevel() {
		this->duration = mTimePerLevel * mSequenceLen;
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
	unsigned char hueLength;
	bool reverse;
	bool gradient;
	EffectParticle() {
		this->duration = BPM128;
		reverse = false;
	}
	virtual void Draw(LedStrip* strip) override;
};

class EffectParticle2 : public EffectStrip
{
public:
	int streakSize;
	bool reverse;
	int offsetH;
	int offsetS;
	int offsetV;
	EffectParticle2() {
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
	int ledOffset = 0;
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
	nSegments = 12;
	nFlashes = 2;
  }
  int seed;
  int nSegments;
  int nFlashes;
  virtual void Draw(LedStrip* strip) override;
};

class EffectFill : public EffectStrip
{
public:
  EffectFill() {
    this->duration = BPM128;
  }
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
