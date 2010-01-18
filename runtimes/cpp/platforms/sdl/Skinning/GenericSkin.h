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
		int getWindowWidth() const;
		int getWindowHeight() const;
		void drawDevice() const;
		void drawScreen() const;
		void rotateCW();
		void rotateCCW();
		void mouseDragged(int x, int y);
		void mousePressed(int x, int y);
		void mouseReleased(int x, int y);

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

		SDL_Rect screenRect;
		bool mTouchedInside;

		SDL_Surface* unselectedPhone;
		SDL_Surface* selectedPhone;

		std::vector<KeyRect> keyRects;
		std::vector<KeyRect> customRects;
		
		int mPressedKey;
	};

	class GenericSkinFactory : public SkinFactory {
	public:
		DeviceSkin* createInstanceFor(const DeviceProfile* dp) const;
		SkinFactory::Fitness getFitnessFor(const DeviceProfile* dp) const;
	};
}

#endif // _GENERICSKIN_H_
