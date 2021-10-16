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
	
	wipe.colorHsv = CHSV(hue, 255, 255);
	wipe.invert = 0;
	unsigned char originalHue = wipe.forceColorHsv.h;
	
	wipe.ledOffset = 0;
	wipe.Draw(outer);
	wipe.ledOffset = inner->Size() - 4;
	wipe.Draw(inner);

	wipe.forceColorHsv.h += 111;
	wipe.colorHsv = CHSV(hue + 111, 255, 255);
	wipe.invert = 1;

	wipe.ledOffset = 0;
	wipe.Draw(outer);
	wipe.ledOffset = inner->Size() - 4;
	wipe.Draw(inner);

	wipe.forceColorHsv.h = originalHue;
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
	unsigned char originalHue = particle.forceColorHsv.h;
	for (size_t i = 0; i < 4; i++)
	{
		particle.colorHsv.h = 128 * i + mapFloat(color.Progress(), 0, color.duration, 0, 255);
		particle.colorHsv.s = 255;
		particle.colorHsv.v = 255;
		particle.forceColorHsv.h = originalHue + 128 * i;

		particle.reverse = false;
		particle.streakSize = 25;
		particle.progessOffset = (particle.duration / 4) * i;
		particle.Draw(outer);

		particle.streakSize = 5;
		particle.progessOffset += particle.duration / 10;
		particle.Draw(inner);
	}
	particle.forceColorHsv.h = originalHue;
}

void AniConfetti::DrawImpl(unsigned long time)
{
	unsigned char hueOffset;
	if (!hue.forceColor)
		hueOffset = mapFloat(hue.Progress(), 0, hue.duration, 0, 255);
	else
		hueOffset = hue.forceColorHsv.h;
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

	particle.colorHsv = CHSV(hueOffset, 255, 255);
	particle.hueLength = 64;
	particle.reverse = false;
	particle.streakSize = outer->Size() / 2;
	particle.progessOffset = 0;
	particle.Draw(outer);
	particle.progessOffset = particle.duration / 2;
	particle.Draw(outer);

	// particle2.hue = hueOffset + HUE_PURPLE;
	// particle2.reverse = true;
	// particle2.streakSize = inner->Size()/2;
	fill.colorHsv = CHSV(hueOffset + 64, 255, 255);
	fill.Draw(inner);
}

void AniSparkle::DrawImpl(unsigned long time)
{
	rainbow.mode = 1;
	rainbow.Draw(leds);
}

void AniBreathe::DrawImpl(unsigned long time)
{
	unsigned char hue = mapFloat(color.Progress(), 0, color.duration, 0, 255);
	breathe.colorHsv = CHSV(hue, 255, 255);
	breathe.Draw(leds);
}

void AniFill::DrawImpl(unsigned long time)
{
	unsigned char hue = mapFloat(color.Progress(), 0, color.duration, 0, 255);
	fill.colorHsv = CHSV(hue, 255, 255);
	fill.Draw(leds);
}

void AniRainbow::DrawImpl(unsigned long time)
{
	outer->Clear();
	inner->Clear();
	particle.gradient = true;
	{
		particle.colorHsv = CHSV(0, 255, 255);
		particle.hueLength = 255;
		particle.reverse = true;
		particle.streakSize = outer->Size() / 2;
		particle.progessOffset = 0;
		particle.Draw(outer);
		particle.progessOffset += particle.duration/2;
		particle.Draw(outer);

		particle.streakSize = inner->Size() / 2;
		particle.progessOffset = -particle.duration/10;
		particle.Draw(inner);
		particle.progessOffset += particle.duration/2;
		particle.Draw(inner);
	}
}

void AniMultiParticle::DrawImpl(unsigned long time)
{
	const int particleCount = 3;
	particle.gradient = false;
	for (size_t j = 0; j < 2; j++)
	{
		LedStrip* ledStrip = leds[j];
		ledStrip->Clear();
		for (size_t i = 0; i < particleCount; i++)
		{
			int hueOffset = mapFloat(color.Progress(), 0, color.duration, 0, 255);
			particle.colorHsv = CHSV(HUE_RED + hueOffset, 255, 128);
			particle.streakSize = ledStrip->Size() / (particleCount*1.5);
			particle.progessOffset = (particle.duration / particleCount) * i;

			ledStrip->SetBlendMode(LedStrip::BLEND_MODE_ADD);
			particle.reverse = false;
			particle.Draw(ledStrip);

			particle.reverse = true;
			unsigned char originalHue = particle.forceColorHsv.h;
			particle.colorHsv.h += HUE_GREEN;
			particle.forceColorHsv.h += HUE_GREEN;
			particle.Draw(ledStrip);
			particle.forceColorHsv.h = originalHue;
			ledStrip->SetBlendMode(LedStrip::BLEND_MODE_NONE);
		}
	}
}

template <int N>
void AniFire<N>::DrawImpl(unsigned long time)
{
	const int cooling = 55;
	const int sparking = 100;
	int hueOffset = mapFloat(hueShift.Progress(), 0, hueShift.duration, 0, 3);
	if (lastHueOffset != hueOffset)
		for( int i = 0; i < outer->Size(); i++)
			heat[i] = 0;

	lastHueOffset = hueOffset;

  	// Step 1.  Cool down every cell a little
    for( int i = 0; i < outer->Size(); i++) {
		heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / outer->Size()) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= outer->Size() - 1; k >= 2; k--) {
		heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < sparking ) {
		int y = random8(7);
		heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < outer->Size(); j++) {
		CRGB color = HeatColor(heat[j]);
		CHSV chsvColor = RgbToHsv(color);
		chsvColor.h += hueOffset*84;
		outer->Leds()[j]= chsvColor; 
		if (j%3 == 0)
			inner->Leds()[j/3]= chsvColor;
    }
}

void AniWheel::DrawImpl(unsigned long time)
{
	const int divisions = 3;
	outer->Clear();
	inner->Clear();
	unsigned char hueOffset;
	if (color.Progress() < color.duration / 2) 
		hueOffset = mapFloat(color.Progress(), 0, color.duration, HUE_BLUE, 300);
	else
		hueOffset = mapFloat(color.Progress(), 0, color.duration, 300, HUE_BLUE);

	particle.colorHsv = CHSV(hueOffset, 200, 255);
	particle.offsetH = 0;
	particle.offsetS = -150;
	particle.offsetV = -100;
	particle.reverse = false;
	particle.streakSize = outer->Size() / divisions;
	for (size_t i = 0; i < divisions; i++)
	{
		particle.progessOffset = i * particle.duration / divisions;
		particle.Draw(outer);
	}

	fill.colorHsv = CHSV(hueOffset, 200, 255);
	fill.Draw(inner);
}


void AniServo::DrawImpl(unsigned long time)
{
	servoLevel.Draw(motor);
}
