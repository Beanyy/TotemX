#include <FastLED.h>
#include "animations.hpp"
#include "helpers.hpp"

void AniWipe::DrawImpl(unsigned long time)
{
	inner->Clear();
	outer->Clear();

	int progress = wipe.Progress();
	if ((wipe.speed > 0 && progress < lastProgress) || (wipe.speed < 0 && progress > lastProgress))
		hue += 144;

	lastProgress = progress;
	wipe.ledOffset = mapFloat(spin.Progress(), 0, spin.duration, 0, inner->Size());
	wipe.color = CHSV(hue, 255, 100);
	wipe.invert = 0;
	wipe.Draw(inner);
	wipe.Draw(outer);

	wipe.ledOffset = mapFloat(spin.Progress(), 0, spin.duration, 0, outer->Size());
	wipe.color = CHSV(hue + 111, 255, 100);
	wipe.invert = 1;
	wipe.Draw(inner);
	wipe.Draw(outer);
}

void AniFlash::DrawImpl(unsigned long time)
{
	flash.seed = 10;
	flash.Draw(outer);
	flash.seed = 11;
	flash.Draw(inner);
}

void AniParticle::DrawImpl(unsigned long time)
{
	outer->Clear();
	inner->Clear();
	particle.gradient = false;
	for (size_t i = 0; i < 4; i++)
	{
		particle.hue = 128 * i + mapFloat(color.Progress(), 0, color.duration, 0, 255);

		particle.reverse = true;
		particle.streakSize = 10;
		particle.progessOffset = (particle.duration / 4) * i;
		particle.Draw(outer);

		particle.streakSize = 3;
		particle.progessOffset += particle.duration / 20;
		particle.Draw(inner);
	}
}

void AniConfetti::DrawImpl(unsigned long time)
{
	unsigned char hueOffset = mapFloat(hue.Progress(), 0, hue.duration, 0, 255);
	fadeToBlackBy(leds->Leds(), leds->Size(), 10);
	leds->SetDir(true).SetOffset(0).SetWrap(true).SetViewport(0, leds->Size());

	int pos = random16(leds->Size());
	leds->SetLED(pos, CHSV(hueOffset + random8(64), 200, 255));
	pos = random16(leds->Size());
	leds->SetLED(pos, CHSV(hueOffset + random8(64), 200, 255));
}

void AniZoom::DrawImpl(unsigned long time)
{
	outer->Clear();
	inner->Clear();
	unsigned char hueOffset = mapFloat(color.Progress(), 0, color.duration, 0, 255);
	particle.gradient = true;

	particle.hueBegin = hueOffset;
	particle.hue = hueOffset + 64;
	particle.reverse = false;
	particle.streakSize = outer->Size() / 2;
	particle.progessOffset = 0;
	particle.Draw(outer);
	particle.progessOffset = particle.duration / 2;
	particle.Draw(outer);

	// particle2.hue = hueOffset + HUE_PURPLE;
	// particle2.reverse = true;
	// particle2.streakSize = inner->Size()/2;
	breathe.hue = hueOffset + 64;
	breathe.Draw(inner);
}

void AniSparkle::DrawImpl(unsigned long time)
{
	rainbow.mode = 1;
	rainbow.Draw(leds);
}

void AniRainbow::DrawImpl(unsigned long time)
{
	particle.gradient = true;
	{
		particle.hueBegin = 0;
		particle.hue = 255;
		particle.reverse = true;
		particle.streakSize = outer->Size() / 2;
		particle.progessOffset = 0;
		particle.Draw(outer);
		particle.progessOffset += particle.duration/2;
		particle.Draw(outer);

		particle.streakSize = inner->Size() / 2;
		particle.progessOffset = particle.duration/10;
		particle.Draw(inner);
		particle.progessOffset += particle.duration/2;
		particle.Draw(inner);
	}
}
