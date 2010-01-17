#ifndef _SCREEN_H_
#define _SCREEN_H_


#include <SDL/SDL.h>

void setWindowSurface(SDL_Surface* s);
SDL_Surface* getWindowSurface();
void setPhoneScreen(SDL_Surface* s);
SDL_Surface* getPhoneScreen();

#endif