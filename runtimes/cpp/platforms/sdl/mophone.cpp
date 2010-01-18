/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "Platform.h"
#include "mophone.h"

#include <SDL/SDL_image.h>

#include <helpers/helpers.h>
#include "sdl_errors.h"
#include "windows_errors.h"

#include "ConfigParser.h"

using namespace MoSyncError;

//********************************************************************
// MOPHONE SPECIFIC
//********************************************************************

using namespace Base;

/*
static Button buttons[NUM_BUTTONS] = {
	{MAK_1, 54, 451, 55, 23},
	{MAK_2, 110, 451, 55, 23},
	{MAK_3, 166, 451, 55, 23},
	{MAK_4, 54, 476, 55, 23},
	{MAK_5, 110, 476, 55, 23},
	{MAK_6, 166, 476, 55, 23},
	{MAK_7, 54, 501, 55, 23},
	{MAK_8, 110, 501, 55, 23},
	{MAK_9, 166, 501, 55, 23},
	{MAK_STAR, 54, 526, 55, 23},
	{MAK_0, 110, 526, 55, 23},
	{MAK_HASH, 166, 526, 55, 23},

	{MAK_UP, 132, 369, 20, 20},
	{MAK_RIGHT, 159, 399, 10, 20},
	{MAK_DOWN, 132, 426, 12, 10},
	{MAK_LEFT, 107, 399, 10, 20},

	{MAK_SOFTLEFT, 32, 378, 80, 23},
	{MAK_SOFTRIGHT, 164, 378, 80, 23},

	{MAK_FIRE, 120, 387, 36, 36},
	{MAK_CLEAR, 222, 500, 26, 60},
};
*/

static Sint16 mophoneScreenX = 18;
static Sint16 mophoneScreenY = 44;
static Uint16 mophoneScreenW = 240;
static Uint16 mophoneScreenH = 320;

static Skin *sSkin = NULL;
static Sint16 mophoneScale = -1;
static Sint16 realScreenStartX = 0;
static Sint16 realScreenStartY = 0;

static bool hasMophone = false;

int containsButtonMophone(int x, int y) {
	if(!hasMophone) return -1;

	x/=mophoneScale;
	y/=mophoneScale;
	for(size_t i = 0; i < sSkin->keys.size(); i++) {
		if((x>=sSkin->keys[i].x) && (x<=sSkin->keys[i].x+sSkin->keys[i].w)&&
			(y>=sSkin->keys[i].y) && (y<=sSkin->keys[i].y+sSkin->keys[i].h)){
				return sSkin->keys[i].keyCode;
		}	
	}
	return -1;
}

int calculateScaleOfMophone(int w, int h) {
	int scale1 = 1, scale2 = 1;
	if(w>=mophoneScreenW) {
		scale1 = ((w-1)/mophoneScreenW)+1;
	}

	if(h>=mophoneScreenH) {
		scale2 = ((h-1)/mophoneScreenH)+1;
	}
	if(scale1>scale2) return scale1;
	else return scale2;
}

int getMophoneRealScreenStartX() {
	return realScreenStartX;
}

int getMophoneRealScreenStartY() {
	return realScreenStartY;
}

int getMophoneRealScreenEndX() {
	return realScreenStartX + mophoneScreenW*mophoneScale;
}

int getMophoneRealScreenEndY() {
	return realScreenStartY + mophoneScreenH*mophoneScale;
}

int getMophoneScale() {
	if(hasMophone)
		return mophoneScale;
	else
		return 1;
}

bool isPointInsideScreen(int x, int y, int w, int h) {
	if(	x>=realScreenStartX && x<=((realScreenStartX+w)) &&
		y>=realScreenStartY && y<=((realScreenStartY+h))) {
		return true;
	} else {
		return false;
	}
}

void calculateMophone(int w, int h) {
	mophoneScale = calculateScaleOfMophone(w, h);
	realScreenStartX = (mophoneScreenX+(mophoneScreenW>>1))*mophoneScale - (w>>1);
	realScreenStartY = (mophoneScreenY+(mophoneScreenH>>1))*mophoneScale - (h>>1);
}

SDL_Surface *mophone_unselected = 0;
SDL_Surface *mophone_selected = 0;

bool initMophoneScreen(Skin* skin, SDL_Surface **gScreen, int w, int h, bool shouldHaveMophone) {
	int lastMophoneScale = mophoneScale;

	sSkin = skin;

	if(sSkin) {
		mophoneScreenX = sSkin->screenX;
		mophoneScreenY = sSkin->screenY;
		mophoneScreenW = sSkin->screenW;
		mophoneScreenH = sSkin->screenH;
	}

	SDL_Surface *surf;
	bool converted = true;

	hasMophone = shouldHaveMophone;

	if(shouldHaveMophone && mophone_unselected == 0) {
		/*
		const char *dir = getenv("MOSYNCDIR");
		char temp[1024];
		sprintf(temp, "%s/bin/mophone.png", dir);
		*/
		SDL_RWops* rwops = SDL_RWFromFile(sSkin->unselectedImage.c_str(), "rb");
		if(!rwops) {
			hasMophone = false;
			LOG("Failed to load %s", sSkin->unselectedImage.c_str());
		} else hasMophone = true;

		if(hasMophone) {
			surf = IMG_LoadPNG_RW(rwops);
			SDL_RWclose(rwops);
			MYASSERT(surf, SDLERR_IMAGE_LOAD_FAILED);
			mophone_unselected = surf;
			//sprintf(temp, "%s/bin/mophone2.png", dir);
			rwops = SDL_RWFromFile(sSkin->selectedImage.c_str(), "rb");
			if(rwops) {
				surf = IMG_LoadPNG_RW(rwops);
				SDL_RWclose(rwops);
				MYASSERT(surf, SDLERR_IMAGE_LOAD_FAILED);
				mophone_selected = surf;
			} else {
				hasMophone = false;
				LOG("Failed to load %s", sSkin->selectedImage.c_str());
			}
		}

		converted = false;
	}

	if(hasMophone) {
		calculateMophone(w, h);
	} else {
		realScreenStartX = 0;
		realScreenStartY = 0;
		mophoneScale = ~lastMophoneScale;
	}

	if(mophoneScale != lastMophoneScale) {            
		if(hasMophone) {
			// fix with mobile image
			TEST_Z(*gScreen = SDL_SetVideoMode(mophone_unselected->w*mophoneScale,
				mophone_unselected->h*mophoneScale, 32, SDL_SWSURFACE | SDL_ANYFORMAT)); 
		} else {
#ifdef __USE_FULLSCREEN__
			TEST_Z(*gScreen = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE | SDL_ANYFORMAT | SDL_FULLSCREEN));
#else
			TEST_Z(*gScreen = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE | SDL_ANYFORMAT ));
#endif
		}
	}

	if(hasMophone) {
		if(!converted) {
			surf = mophone_unselected;
			mophone_unselected = SDL_DisplayFormat(surf);
			SDL_FreeSurface(surf);
	
			surf = mophone_selected;
			mophone_selected = SDL_DisplayFormat(surf);
			SDL_FreeSurface(surf);
		}

		SDL_Rect srcRect = {0, 0, (Uint16)mophone_unselected->w, (Uint16)mophone_unselected->h};
		pixelDoubledBlit(0, 0, *gScreen, mophone_unselected, srcRect, mophoneScale);	
	
		SDL_Rect dstRect = {(Sint16)(mophoneScreenX*mophoneScale), (Sint16)(mophoneScreenY*mophoneScale), 
			(Uint16)(mophoneScreenW*mophoneScale), (Uint16)(mophoneScreenH*mophoneScale)};
		SDL_FillRect(*gScreen, &dstRect, 0xff6f6f6f);
		SDL_UpdateRect(*gScreen, dstRect.x, dstRect.y, dstRect.w, dstRect.h);
	}
	return hasMophone;
}

void drawButtonMophone(SDL_Surface *gScreen, int mak, bool pressed) {
	if(!hasMophone) return;

	for(size_t i = 0; i < sSkin->keys.size(); i++) {
		if(sSkin->keys[i].keyCode == mak) {
			SDL_Rect srcRect = {
				(Sint16)sSkin->keys[i].x,
				(Sint16)sSkin->keys[i].y,
				(Uint16)sSkin->keys[i].w,
				(Uint16)sSkin->keys[i].h
			};

			if(pressed) {
				pixelDoubledBlit(
					sSkin->keys[i].x*mophoneScale, 
					sSkin->keys[i].y*mophoneScale, 
					gScreen, 
					mophone_selected, 
					srcRect,
					mophoneScale);	
			} else {
				pixelDoubledBlit(
					sSkin->keys[i].x*mophoneScale, 
					sSkin->keys[i].y*mophoneScale, 
					gScreen, 
					mophone_unselected, 
					srcRect,
					mophoneScale);	
			}

			SDL_UpdateRect(gScreen, 
				sSkin->keys[i].x*mophoneScale, 
				sSkin->keys[i].y*mophoneScale,
				sSkin->keys[i].w*mophoneScale,
				sSkin->keys[i].h*mophoneScale);
		}
	}
}

void freeMophone() {
	if(!hasMophone) return;

	if(!mophone_unselected) return;
	SDL_FreeSurface(mophone_unselected);
	SDL_FreeSurface(mophone_selected);
}
