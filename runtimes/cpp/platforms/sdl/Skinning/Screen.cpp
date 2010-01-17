#include "Screen.h"


static SDL_Surface* sPhoneScreen = 0;
static SDL_Surface* sWindowSurface = 0;

void setWindowSurface(SDL_Surface* s) {
	sWindowSurface = s;
}

SDL_Surface* getWindowSurface() {
	return sWindowSurface;
}

void setPhoneScreen(SDL_Surface* s) {
	sPhoneScreen = s;
}

SDL_Surface* getPhoneScreen() {
	return sPhoneScreen;
}
