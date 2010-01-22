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

#include <SDL/SDL_image.h>

#include "GenericSkin.h"
#include "Screen.h"

#include "helpers/cpp_defs.h"

#define LOG printf

namespace MoRE {
	bool KeyRect::contains(int lx, int ly) {
		if(lx>=this->x && lx<this->x+this->w && ly>=this->y && ly<this->y+this->h) return true;
		return false;
	}

	SDL_Surface* GenericSkin::sSkinImage = 0;
	SDL_Surface* GenericSkin::sSelectedKeypad = 0;
	SDL_Surface* GenericSkin::sUnselectedKeypad = 0;

	static SDL_Surface* createSurface(int width,int height) {
		Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif

		//return SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA, width, height, 32, rmask, gmask, bmask, amask );

		SDL_Surface* first = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA, width, height, 32, rmask, gmask, bmask, amask);
		SDL_SetAlpha(first, 0,255);
		//SDL_Surface* second = SDL_DisplayFormatAlpha(first);
		return first;
	}

	static SDL_Surface* loadPNGImage(const char* name) {
		SDL_RWops* rwops = SDL_RWFromFile(name, "rb");
		if(!rwops) {
			LOG("Failed to load %s", name);
			return 0;
		}
		SDL_Surface* surf = IMG_LoadPNG_RW(rwops);
		SDL_RWclose(rwops);
		//MYASSERT(surf, SDLERR_IMAGE_LOAD_FAILED);
		return surf;
	}

	GenericSkin::GenericSkin(const DeviceProfile* profile) 
		:
		DeviceSkin(profile),
		mPressedKey(0),
		mTouchedInside(false)
	{
	}

	bool GenericSkin::init() {
		const char *dir = getenv("MOSYNCDIR");
		
		if(!dir) return false;

		if(!sSkinImage) 
			sSkinImage        = loadPNGImage((std::string(dir)+"/skins/skinImage.png").c_str());
		if(!sSkinImage) return false;
		if(!sSelectedKeypad) 
			sSelectedKeypad   = loadPNGImage((std::string(dir)+"/skins/keypadSelected.png").c_str());
		if(!sSelectedKeypad) return false;
		if(!sUnselectedKeypad) 
			sUnselectedKeypad = loadPNGImage((std::string(dir)+"/skins/keypadUnselected.png").c_str());
		if(!sUnselectedKeypad) return false;

		int w = getWindowWidth();
		int h = getWindowHeight();

		unselectedPhone = createSurface(w, h);
		selectedPhone   = createSurface(w, h);

		generatePhone();
		
		int keysX=0, keysY=0;

		if(mProfile->mKeyboardType == DeviceProfile::DKT_KEYPAD) {
			keysX = w/2 - sUnselectedKeypad->w/2;
			keysY = h - sUnselectedKeypad->h - 24;
		} else if(mProfile->mKeyboardType == DeviceProfile::DKT_JOYSTICK) {
			keysX = w/2 - sUnselectedKeypad->w/2;
			keysY = h - 74 - 24;	
		}
	
#define OFFSET_X (27-keysX)
#define OFFSET_Y (368-keysY)
		if(mProfile->mKeyboardType == DeviceProfile::DKT_JOYSTICK || mProfile->mKeyboardType == DeviceProfile::DKT_KEYPAD) {

			keyRects.push_back(KeyRect(MAK_UP, 132-OFFSET_X, 369-OFFSET_Y, 20, 20));
			keyRects.push_back(KeyRect(MAK_RIGHT, 159-OFFSET_X, 399-OFFSET_Y, 10, 20));
			keyRects.push_back(KeyRect(MAK_DOWN, 132-OFFSET_X, 426-OFFSET_Y, 12, 10));
			keyRects.push_back(KeyRect(MAK_LEFT, 107-OFFSET_X, 399-OFFSET_Y, 10, 20));

			keyRects.push_back(KeyRect(MAK_SOFTLEFT, 32-OFFSET_X, 378-OFFSET_Y, 80, 23));
			keyRects.push_back(KeyRect(MAK_SOFTRIGHT, 164-OFFSET_X, 378-OFFSET_Y, 80, 23));

			keyRects.push_back(KeyRect(MAK_FIRE, 120-OFFSET_X, 387-OFFSET_Y, 36, 36));
			keyRects.push_back(KeyRect(MAK_CLEAR, 222-OFFSET_X, 500-OFFSET_Y, 26, 60));
		}

		if(mProfile->mKeyboardType == DeviceProfile::DKT_KEYPAD) {
			keyRects.push_back(KeyRect(MAK_1, 54-OFFSET_X, 451-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_2, 110-OFFSET_X, 451-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_3, 166-OFFSET_X, 451-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_4, 54-OFFSET_X, 476-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_5, 110-OFFSET_X, 476-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_6, 166-OFFSET_X, 476-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_7, 54-OFFSET_X, 501-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_8, 110-OFFSET_X, 501-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_9, 166-OFFSET_X, 501-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_STAR, 54-OFFSET_X, 526-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_0, 110-OFFSET_X, 526-OFFSET_Y, 55, 23));
			keyRects.push_back(KeyRect(MAK_HASH, 166-OFFSET_X, 526-OFFSET_Y, 55, 23));
		}

		screenRect.x = w/2 - mProfile->mScreenWidth/2;
		screenRect.y = 24;
		screenRect.w = mProfile->mScreenWidth;
		screenRect.h = mProfile->mScreenHeight;	

		windowRect.x = 0;
		windowRect.y = 0;
		windowRect.w = w;
		windowRect.h = h;

		return true;
	}

	int GenericSkin::getWindowWidth() const {
		int w =  mProfile->mScreenWidth + 48;
		return w > 260 ? w : 260;
	}
	
	int GenericSkin::getWindowHeight() const {
		int extra = 0;

		switch(mProfile->mKeyboardType) {
			case DeviceProfile::DKT_NONE :
			extra = 48;
			break;
			case DeviceProfile::DKT_JOYSTICK :
			extra = 140;
			break;
			case DeviceProfile::DKT_KEYPAD :
			extra = 240;
			break;
			case DeviceProfile::DKT_QWERTY :
			extra = 300;
			break;
		}
		return mProfile->mScreenHeight + extra;
	}

	/*
	static void fillRect(int left, int top, int width, int height, unsigned int color) {
		SDL_Rect rect = { left,top,width,height };
		SDL_FillRect(getWindowSurface(), &rect, color);
	}
	*/

	void GenericSkin::drawDevice() const {
		//generatePhone();
		/*
		int width  = getWindowWidth();
		int height = getWindowHeight();
		*/

		// draw background
		//skinPhone(getWindowSurface());

		// draw buttons
		/*
		if(mProfile->mKeyboardType == DeviceProfile::DKT_KEYPAD) {
			SDL_Rect srcRect, dstRect;
			srcRect.x = 0;
			srcRect.y = 0;
			dstRect.w = srcRect.w = sUnselectedKeypad->w;
			dstRect.h = srcRect.h = sUnselectedKeypad->h;
			dstRect.x = width/2 - sUnselectedKeypad->w/2;
			dstRect.y = height - sUnselectedKeypad->h - 24;
			SDL_BlitSurface(sUnselectedKeypad, &srcRect, getWindowSurface(), &dstRect);
		}
		else if(mProfile->mKeyboardType == DeviceProfile::DKT_JOYSTICK) {
			SDL_Rect srcRect, dstRect;
			srcRect.x = 0;
			srcRect.y = 0;
			dstRect.w = srcRect.w = sUnselectedKeypad->w;
			dstRect.h = srcRect.h = 74;
			dstRect.x = width/2 - sUnselectedKeypad->w/2;
			dstRect.y = height - 74 - 24;
			SDL_BlitSurface(sUnselectedKeypad, &srcRect, getWindowSurface(), &dstRect);
		}
		*/

		SDL_Rect clipRect;
		SDL_GetClipRect(getWindowSurface(), &clipRect);
		SDL_SetClipRect(getWindowSurface(), &windowRect);

		int err;
		if((err=SDL_BlitSurface(unselectedPhone, NULL, getWindowSurface(), NULL)) != 0) {
			char* errStr = SDL_GetError();
			printf("ERROR BLITTING: %s!!!!\n", errStr);
		}

		SDL_UpdateRect(getWindowSurface(), windowRect.x, windowRect.y, windowRect.w, windowRect.h);

		SDL_SetClipRect(getWindowSurface(), &clipRect);

		//Base::pixelDoubledBlit(0, 0, getWindowSurface(), unselectedPhone, unselectedPhone->clip_rect, 1);
				
		// draw screen

		//fillRect(width/2 - mProfile->mScreenWidth/2, 24, mProfile->mScreenWidth, mProfile->mScreenHeight, 0xff00ff);

	}

	void GenericSkin::drawScreen() const {
		//int width  = getWindowWidth();
		//fillRect(width/2 - mProfile->mScreenWidth/2, 24, mProfile->mScreenWidth, mProfile->mScreenHeight, 0xff00ff);
		
		SDL_Rect clipRect;
		SDL_GetClipRect(getWindowSurface(), &clipRect);
		SDL_SetClipRect(getWindowSurface(), &windowRect);

		if(SDL_BlitSurface(getPhoneScreen(), NULL, getWindowSurface(), (SDL_Rect*) &screenRect) != 0) {
			printf("ERROR BLITTING!!!!\n");
		}

		SDL_UpdateRect(getWindowSurface(), screenRect.x, screenRect.y, screenRect.w, screenRect.h);
		SDL_SetClipRect(getWindowSurface(), &clipRect);
	}

	void GenericSkin::skinPhone(SDL_Surface* surface, SDL_Surface* keypad) const {
		//surface = getWindowSurface();
		
		SDL_Rect srcRect;
		SDL_Rect dstRect;

		int width  = getWindowWidth();
		int height = getWindowHeight();

		// topleft
		srcRect.x = 0;  srcRect.y = 0;
		srcRect.w = 24; srcRect.h = 24;
		dstRect.x = 0;  dstRect.y = 0;
		dstRect.w = 24; dstRect.h = 24;
		SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);

		// topright
		srcRect.x = 40;  srcRect.y = 0;
		srcRect.w = 24; srcRect.h = 24;
		dstRect.x = width - 24;  dstRect.y = 0;
		dstRect.w = 24; dstRect.h = 24;
		SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);

		// bottomleft
		srcRect.x = 0;  srcRect.y = 40;
		srcRect.w = 24; srcRect.h = 24;
		dstRect.x = 0;  dstRect.y = height-24;
		dstRect.w = 24; dstRect.h = 24;
		SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);

		// bottomright
		srcRect.x = 40;  srcRect.y = 40;
		srcRect.w = 24; srcRect.h = 24;
		dstRect.x = width - 24;  dstRect.y = height - 24;
		dstRect.w = 24; dstRect.h = 24;
		SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);

		// Fill top

		int steps = (width - 48) / 16;
		int remainder = (width - 48) % 16;

		srcRect.x = 24; srcRect.y = 0;
		srcRect.w = 16; srcRect.h = 24;
		dstRect.x = 24; dstRect.y = 0;

		for(int i = 0; i < steps; i++) {
			SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
			dstRect.x += 16;
		}
		// remainder

		if(remainder != 0) {
			srcRect.w = remainder;
			SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
		}

		// Fill bottom

		srcRect.x = 24; srcRect.y = 40;
		srcRect.w = 16; srcRect.h = 24;
		dstRect.x = 24; dstRect.y = height - 24;

		for(int i = 0; i < steps; i++) {
			SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
			dstRect.x += 16;
		}
		// remainder

		if(remainder != 0) {
			srcRect.w = remainder;
			SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
		}

		// Fill left

		steps = (height - 48) / 16;
		remainder = (height - 48) % 16;

		srcRect.x = 0; srcRect.y = 24;
		srcRect.w = 24; srcRect.h = 16;
		dstRect.x = 0; dstRect.y = 24;

		for(int i = 0; i < steps; i++) {
			SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
			dstRect.y += 16;
		}
		// remainder

		if(remainder != 0) {
			srcRect.h = remainder;
			SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
		}

		// Fill right

		srcRect.x = 40; srcRect.y = 24;
		srcRect.w = 24; srcRect.h = 16;
		dstRect.x = width - 24; dstRect.y = 24;

		for(int i = 0; i < steps; i++) {
			SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
			dstRect.y += 16;
		}
		// remainder

		if(remainder != 0) {
			srcRect.h = remainder;
			SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
		}

		// fill inside

		srcRect.x = 24; srcRect.y = 24;
		srcRect.w = 16; srcRect.h = 16;
		dstRect.x = 24; dstRect.y = 24;

		int xsteps = (width - 48) / 16;
		int xremainder = (width - 48) % 16;
		int ysteps = (height - 48) / 16;
		int yremainder = (height - 48) % 16;

		printf("xsteps: %d\n", xsteps);
		printf("ysteps: %d\n", ysteps);

		for(int i = 0; i < ysteps; i++) {
			for(int j = 0; j < xsteps; j++) {
				SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
				dstRect.x += 16;
			}
			if(xremainder != 0) {
				srcRect.w = xremainder;
				SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
			}
			dstRect.y += 16;
			dstRect.x = 24;
			srcRect.w = 16;
		}

		if(yremainder != 0) {
			srcRect.h = yremainder;
			for(int j = 0; j < xsteps; j++) {
				SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
				dstRect.x += 16;
			}
			if(xremainder != 0) {
				srcRect.w = xremainder;
				SDL_BlitSurface(sSkinImage, &srcRect, surface, &dstRect);
			}
			dstRect.y += 16;
			dstRect.x = 24;
			srcRect.w = 16;
		}

		if(mProfile->mKeyboardType == DeviceProfile::DKT_KEYPAD) {
			SDL_Rect srcRect2, dstRect2;
			srcRect2.x = 0;
			srcRect2.y = 0;
			dstRect2.w = srcRect2.w = sUnselectedKeypad->w;
			dstRect2.h = srcRect2.h = sUnselectedKeypad->h;
			dstRect2.x = width/2 - sUnselectedKeypad->w/2;
			dstRect2.y = height - sUnselectedKeypad->h - 24;
			SDL_BlitSurface(keypad, &srcRect2, surface, &dstRect2);
		}
		else if(mProfile->mKeyboardType == DeviceProfile::DKT_JOYSTICK) {
			SDL_Rect srcRect2, dstRect2;
			srcRect2.x = 0;
			srcRect2.y = 0;
			dstRect2.w = srcRect2.w = sUnselectedKeypad->w;
			dstRect2.h = srcRect2.h = 74;
			dstRect2.x = width/2 - sUnselectedKeypad->w/2;
			dstRect2.y = height - 74 - 24;
			SDL_BlitSurface(keypad, &srcRect, surface, &dstRect);
		}

	}

	void GenericSkin::generatePhone() {

		skinPhone(unselectedPhone, sUnselectedKeypad);
		skinPhone(selectedPhone, sSelectedKeypad);
		/*
		int width  = getWindowWidth();
		int height = getWindowHeight();
		*/

		// draw buttons
		/*
		if(mProfile->mKeyboardType == DeviceProfile::DKT_KEYPAD) {
			SDL_Rect srcRect, dstRect;
			srcRect.x = 0;
			srcRect.y = 0;
			dstRect.w = srcRect.w = sUnselectedKeypad->w;
			dstRect.h = srcRect.h = sUnselectedKeypad->h;
			dstRect.x = width/2 - sUnselectedKeypad->w/2;
			dstRect.y = height - sUnselectedKeypad->h - 24;
			SDL_BlitSurface(sUnselectedKeypad, &srcRect, unselectedPhone, &dstRect);
		}
		else if(mProfile->mKeyboardType == DeviceProfile::DKT_JOYSTICK) {
			SDL_Rect srcRect, dstRect;
			srcRect.x = 0;
			srcRect.y = 0;
			dstRect.w = srcRect.w = sUnselectedKeypad->w;
			dstRect.h = srcRect.h = 74;
			dstRect.x = width/2 - sUnselectedKeypad->w/2;
			dstRect.y = height - 74 - 24;
			SDL_BlitSurface(sUnselectedKeypad, &srcRect, unselectedPhone, &dstRect);
		}
		*/

	}
	
	void GenericSkin::rotateCW() {
	}
	
	void GenericSkin::rotateCCW() {
	
	}
	
	void GenericSkin::mouseDragged(int x, int y) {
		if(mPressedKey) return;

		if( x>=screenRect.x && x<screenRect.x+screenRect.w &&
			y>=screenRect.y && y<screenRect.y+screenRect.h) {
				if(!mTouchedInside) {
					mousePressed(x, y);
					return;
				}

				mListener->onMoSyncPointerDrag(x-screenRect.x, y-screenRect.y);
				return;
		} else {
			if(mTouchedInside) {
				if(x<screenRect.x) x = screenRect.x;
				if(y<screenRect.y) y = screenRect.y;
				if(x>=screenRect.x+screenRect.w) x = screenRect.x+screenRect.w-1;
				if(y>=screenRect.y+screenRect.h) x = screenRect.y+screenRect.h-1;
				mouseReleased(x, y);
				return;
			}
		}
	}
	
	void GenericSkin::mousePressed(int x, int y) {
		if( x>=screenRect.x && x<screenRect.x+screenRect.w &&
			y>=screenRect.y && y<screenRect.y+screenRect.h) {
				mListener->onMoSyncPointerPress(x-screenRect.x, y-screenRect.y);
				mTouchedInside = true;
				return;
		}

		mTouchedInside = false;

		for(size_t i = 0; i < keyRects.size(); i++) {
			if(keyRects[i].contains(x, y)) {
				mListener->onMoSyncKeyPress(keyRects[i].keyCode);
				mPressedKey = keyRects[i].keyCode;
				keyPressed(mPressedKey);
				return;
			}
		}
	}
	
	void GenericSkin::mouseReleased(int x, int y) {
		if(mTouchedInside) {
			if( x>=screenRect.x && x<screenRect.x+screenRect.w &&
				y>=screenRect.y && y<screenRect.y+screenRect.h) {
					mListener->onMoSyncPointerRelease(x-screenRect.x, y-screenRect.y);
					return;
			}
			mTouchedInside = false;
			return;
		}

		if(!mPressedKey) return;
		mListener->onMoSyncKeyRelease(mPressedKey);
		keyReleased(mPressedKey);
		mPressedKey = 0;
	}

	void GenericSkin::keyPressed(int mak) {
		for(size_t i = 0; i < keyRects.size(); i++) {
			if(keyRects[i].keyCode == mak) {
				SDL_Rect clipRect;
				SDL_GetClipRect(getWindowSurface(), &clipRect);
				SDL_SetClipRect(getWindowSurface(), &windowRect);

				SDL_Rect src = { (Sint16)keyRects[i].x, (Sint16)keyRects[i].y,
					(Uint16)keyRects[i].w, (Uint16)keyRects[i].h };
				SDL_BlitSurface(selectedPhone, &src, getWindowSurface(), &src);
				SDL_UpdateRect(getWindowSurface(), src.x, src.y, src.w, src.h);
				SDL_SetClipRect(getWindowSurface(), &clipRect);
				return;
			}
		}
	}

	void GenericSkin::keyReleased(int mak) {
		for(size_t i = 0; i < keyRects.size(); i++) {
			if(keyRects[i].keyCode == mak) {
				SDL_Rect clipRect;
				SDL_GetClipRect(getWindowSurface(), &clipRect);
				SDL_SetClipRect(getWindowSurface(), &windowRect);

				SDL_Rect src = { (Sint16)keyRects[i].x, (Sint16)keyRects[i].y,
					(Uint16)keyRects[i].w, (Uint16)keyRects[i].h };
				SDL_BlitSurface(unselectedPhone, &src, getWindowSurface(), &src);	
				SDL_UpdateRect(getWindowSurface(), src.x, src.y, src.w, src.h);
				SDL_SetClipRect(getWindowSurface(), &clipRect);
				return;
			}
		}
	}

	DeviceSkin* GenericSkinFactory::createInstanceFor(const DeviceProfile* dp) const {
		return new GenericSkin(dp);
	}
	
	SkinFactory::Fitness GenericSkinFactory::getFitnessFor(const DeviceProfile* dp) const {
		return SkinFactory::SKIN_FITNESS_COMPATIBLE;
	}

	
}
