#include <SDL/SDL_image.h>

#include "GenericSkin.h"
#include "Screen.h"

#define LOG printf

namespace MoRE {

	SDL_Surface* GenericSkin::sSkinImage = 0;
	SDL_Surface* GenericSkin::sSelectedKeypad = 0;
	SDL_Surface* GenericSkin::sUnselectedKeypad = 0;

	SDL_Surface* createSurface(int width,int height) {
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
		SDL_Surface* second = SDL_DisplayFormatAlpha(first);
		return second;
	}

	SDL_Surface* loadPNGImage(const char* name) {
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

	GenericSkin::GenericSkin(DeviceProfile* profile) 
		:
		DeviceSkin(profile)
	{
		if(!sSkinImage) 
			sSkinImage        = loadPNGImage("skinImage.png");
		if(!sSelectedKeypad) 
			sSelectedKeypad   = loadPNGImage("keypadUnselected.png");
		if(!sUnselectedKeypad) 
			sUnselectedKeypad = loadPNGImage("keypadSelected.png");


		int w = getWindowWidth();
		int h = getWindowHeight();

		unselectedPhone = createSurface(w, h);
		selectedPhone   = createSurface(w, h);

		generatePhone();
	}

	int GenericSkin::getWindowWidth() {
		int w =  mProfile->mScreenWidth + 48;
		return w > 260 ? w : 260;
	}
	
	int GenericSkin::getWindowHeight() {
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

	static void fillRect(int left, int top, int width, int height, unsigned int color) {
		SDL_Rect rect = { left,top,width,height };
		SDL_FillRect(getWindowSurface(), &rect, color);
	}

	void GenericSkin::drawDevice() {
		//generatePhone();
		int width  = getWindowWidth();
		int height = getWindowHeight();

		// draw background

		skinPhone(getWindowSurface());

		if(SDL_BlitSurface(selectedPhone, NULL, getWindowSurface(), NULL) != 0) {
			printf("ERROR BLITTING!!!!\n");
		}
				
		// draw screen

		fillRect(width/2 - mProfile->mScreenWidth/2, 24, mProfile->mScreenWidth, mProfile->mScreenHeight, 0xff00ff);

	}

	void GenericSkin::skinPhone(SDL_Surface* surface) {
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

	}

	void GenericSkin::generatePhone() {

		skinPhone(unselectedPhone);
		skinPhone(selectedPhone);

		int width  = getWindowWidth();
		int height = getWindowHeight();

		// draw buttons

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

	}
	
	void GenericSkin::rotateCW() {
	}
	
	void GenericSkin::rotateCCW() {
	
	}
	
	void GenericSkin::mouseMoved(int x, int y) {
	
	}
	
	void GenericSkin::mousePressed(int x, int y, int button) {
	
	}
	
	void GenericSkin::mouseReleased(int x, int y, int button) {
	
	}

	DeviceSkin* GenericSkinFactory::createInstanceFor(DeviceProfile* dp) const {
		return new GenericSkin(dp);
	}
	
	SkinFactory::Fitness GenericSkinFactory::getFitnessFor(DeviceProfile* dp) const {
		return SkinFactory::SKIN_FITNESS_COMPATIBLE;
	}

	
}