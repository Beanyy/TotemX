#ifndef EFFECTBASE_HPP
#define EFFECTBASE_HPP

#include "ledStrip.hpp"

#ifdef SIMULATOR
#include <ctime>
unsigned long millis();
#endif

class Effect
{
private:
	int prog;

public:
	unsigned long lastTime;
	int duration; //Try to make this a number with many factors.
	int progessOffset;
	float speed;
	CHSV color;

	Effect() : duration(0),
			   speed(1.0f)
	{
		Reset();
		color = CHSV(HUE_GREEN, 128, 100);
	}

	virtual void Update(unsigned long time);
	void Reset();
	int Progress();
};

#endif
