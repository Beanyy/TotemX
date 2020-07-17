#include "animations.hpp"

void AniWipe::DrawImpl(unsigned long time)
{
	inner->Clear();
    outer->Clear();

	int progress = wipe.Progress();
	if ((wipe.speed > 0 && progress < lastProgress) || (wipe.speed < 0 && progress > lastProgress))
		hue += 144;

	lastProgress = progress;
  wipe.color = CHSV(hue, 255, 100);
	wipe.invert = 0;
	wipe.Draw(inner);
    wipe.Draw(outer);

	wipe.color = CHSV(hue + 111, 255, 100);
	wipe.invert = 1;
	wipe.Draw(inner);
    wipe.Draw(outer);
}
