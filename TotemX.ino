
#include <WS2812B.h>
#include <FastLED.h>
#include "animations.hpp"
#include "appState.hpp"
#include "servoAnimation.hpp"
#include "Servo.h"

#define MOTOR_DISABLE
#define LEDS_INNER 36
#define LEDS_OUTER 112
#define NUM_ANIMATIONS 1

WS2812B strip = WS2812B(LEDS_INNER + LEDS_OUTER);
CRGB leds[LEDS_INNER + LEDS_OUTER];
LedStrip ledsInner(&leds[0], LEDS_INNER);
LedStrip ledsOuter(&leds[LEDS_INNER], LEDS_OUTER);
LedStrip ledsAll(&leds[0], LEDS_OUTER + LEDS_INNER);
AppState state;

Servo motorFront;
Servo motorBack;
DualServoAnimation servoAnimation(88, 97, 75, 95);

// void sendCommand(const char *cmd)
// {
// 	Serial1.print(cmd);
// 	delay(500);
// 	while (Serial1.available())
// 	{
// 		Serial.write(Serial1.read());
// 	}
// }
void executeCommand(char *cmd)
{
	Serial.println(cmd);
	if (cmd[0] == 'r')
		state.Reset();
	else
		state.Update(cmd);
}

void parseCommand()
{
	static char cmd[16];
	static int i = 0;
	bool cmdValid = false;
	while (Serial2.available())
	{
		cmd[i] = Serial2.read();
		if (cmd[i] == '@')
		{
			cmd[i] = 0;
			i = 0;
			executeCommand(cmd);
			return;
		}
		i++;
	}
}

enum AnimationMode
{
	MODE_NORMAL = 0,
	MODE_SKIP,
	MODE_EXTEND
};

struct AnimationList
{
	int count;
	int current;
	Animation *animation[NUM_ANIMATIONS];
	AnimationMode mode[NUM_ANIMATIONS];
} aniList;

void registerAnimation(Animation *ani, enum AnimationMode mode);
void registerAnimation(Animation *ani, enum AnimationMode mode)
{
	aniList.animation[aniList.count] = ani;
	aniList.mode[aniList.count] = mode;
	aniList.count++;
}

unsigned long curTime;
unsigned long lastSwapTime;
AniWipe aniWipe(&ledsInner, &ledsOuter);
AniFlash aniFlash(&ledsInner, &ledsOuter);
AniParticle aniParticle(&ledsInner, &ledsOuter);
AniZoom aniZoom(&ledsInner, &ledsOuter);
AniRainbow aniRainbow(&ledsInner, &ledsOuter);
AniSparkle aniSparkle(&ledsAll);
AniConfetti aniConfetti(&ledsAll);

void setup()
{
	Serial.begin(9600);
	Serial2.begin(9600);
	strip.begin(); // Sets up the SPI
	strip.show();  // Clears the strip, as by default the strip data is set to all LED's off.

	aniList.count = 0;
	aniList.current = 0;
	for (int i = 0; i < NUM_ANIMATIONS; i++)
		aniList.animation[i] = NULL;
	//registerAnimation(&aniFlash, MODE_EXTEND);
	registerAnimation(&aniZoom, MODE_EXTEND);

	curTime = millis();
	lastSwapTime = curTime;

#ifndef MOTOR_DISABLE
	motorFront.attach(PA1);
	motorBack.attach(PA0);
#endif
}

int getLoopAnimation()
{
	const int skip = 1;
	int swapTime = (aniList.mode[aniList.current] == MODE_EXTEND) ? 20000 : 10000;
	if (curTime - lastSwapTime > swapTime)
	{
		lastSwapTime = curTime;
		for (int i = 0; i < skip; i++)
		{
			do
			{
				aniList.current = (aniList.current + 1) % aniList.count;
			} while (aniList.mode[aniList.current] == MODE_SKIP);
		}
	}
}

void runAnimation(Animation *a)
{
	if (state.colorOverride)
		a->ColorOverride(state.color);

	if (state.motionOverride)
		a->SpeedOverride(state.speed);

	if (state.pause)
		a->Pause();
	else
		a->Resume();

	a->Draw(curTime);
}

void loop()
{
	int minDelay = 16;
	curTime = millis();
	parseCommand();

	int a = (state.animation == -1) ? getLoopAnimation() : state.animation;
	if (a < NUM_ANIMATIONS && aniList.animation[a])
		runAnimation(aniList.animation[a]);

	for (int i = 0; i < LEDS_INNER + LEDS_OUTER; i++) {
		if (!state.off)
			strip.setPixelColor(i, strip.Color(leds[i].r, leds[i].g, leds[i].b));
		else
			strip.setPixelColor(i, strip.Color(0, 0, 0));
	}
	strip.show();

#ifndef MOTOR_DISABLE
	servoAnimation.Draw(curTime);
	motorFront.write(servoAnimation.val[0]);
	motorBack.write(servoAnimation.val[1]);
#endif

	unsigned long duration = millis() - curTime;
	if (duration < minDelay)
		delay(minDelay - duration);
}
