/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _GENERICSKIN_H_
#define _GENERICSKIN_H_

#include "SkinFactory.h"
#include <SDL/SDL.h>

namespace MoRE {

	/**
	 * A generic implementation of a skin, similar to the original 
	 * "mophone" design, but supporting various screen sizes and
	 * input types.
	 */

	struct KeyRect {
		KeyRect(int lkeyCode, int lx, int ly, int lw, int lh)
			: keyCode(lkeyCode), x(lx), y(ly), w(lw), h(lh) {}
		bool contains(int x, int y);
		int keyCode;
		int x, y, w, h;
	};

	class GenericSkin : public DeviceSkin {
	public:
		GenericSkin(const DeviceProfile* profile);
		bool init();
		int getWindowWidth() const;
		int getWindowHeight() const;
		void drawDevice() const;
		void drawScreen() const;
		void drawMultiTouchSimulation() const;
		void rotateCW();
		void rotateCCW();
		void mouseDragged(int x, int y);
		void mousePressed(int x, int y);
		void mouseReleased(int x, int y);
		void mouseMultiDragged(int x, int y);
		void mouseMultiPressed(int x, int y);
		void mouseMultiReleased(int x, int y);
		int getScreenLeft() const;
		int getScreenTop() const;
		int getScreenWidth() const;
		int getScreenHeight() const;

		void keyPressed(int mak);
		void keyReleased(int mak);

	private:

		void addKeyRect(int keyCode, int x, int y, int w, int h);
		void addCustonRect(int id, int x, int y, int w, int h);

		void skinPhone(SDL_Surface* surface, SDL_Surface* keypad) const;
		void generatePhone();

		static SDL_Surface* sSkinImage;
		static SDL_Surface* sSelectedKeypad;
		static SDL_Surface* sUnselectedKeypad;
		static SDL_Surface* sMultiTouchImage;

		SDL_Rect screenRect, windowRect;
		int mPressedKey;
		bool mTouchedInside;

		SDL_Surface* unselectedPhone;
		SDL_Surface* selectedPhone;

		std::vector<KeyRect> keyRects;
		std::vector<KeyRect> customRects;		
		
	};

	class GenericSkinFactory : public SkinFactory {
	public:
		DeviceSkin* createInstanceFor(const DeviceProfile* dp) const;
		SkinFactory::Fitness getFitnessFor(const DeviceProfile* dp) const;
	};
}

#endif // _GENERICSKIN_H_
