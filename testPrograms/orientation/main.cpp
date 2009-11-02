#include <ma.h>
#include <maprofile.h>

enum Orientation {
	Landscape, Portrait
};

static Orientation defaultOrientation() {
	if(MA_PROF_CONST_SCREENSIZE_X > MA_PROF_CONST_SCREENSIZE_Y)
		return Landscape;
	else
		return Portrait;
}

Orientation currentOrientation() {
	MAExtent s = maGetScrSize();
	if(EXTENT_X(s) > EXTENT_Y(s))
		return Landscape;
	else
		return Portrait;
}


enum SoftkeyLocation {
	Bottom, Right
};

SoftkeyLocation currentSoftkeyLocation() {
	if(defaultOrientation() == currentOrientation())
		return Bottom;
	else
		return Right;
}
