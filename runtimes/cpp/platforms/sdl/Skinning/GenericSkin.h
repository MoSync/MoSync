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
		KeyRect(int keyCode, int x, int y, int w, int h)
			: keyCode(keyCode), x(x), y(y), w(w), h(w) {}
		bool contains(int x, int y);
		int keyCode;
		int x, y, w, h;
	};

	class GenericSkin : public DeviceSkin {
	public:
		GenericSkin(DeviceProfile* profile);
		int getWindowWidth();
		int getWindowHeight();
		void drawDevice();
		void rotateCW();
		void rotateCCW();
		void mouseMoved(int x, int y);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
	private:

		void addKeyRect(int keyCode, int x, int y, int w, int h);
		void addCustonRect(int id, int x, int y, int w, int h);

		void skinPhone(SDL_Surface* surface);
		void generatePhone();

		static SDL_Surface* sSkinImage;
		static SDL_Surface* sSelectedKeypad;
		static SDL_Surface* sUnselectedKeypad;

		SDL_Surface* unselectedPhone;
		SDL_Surface* selectedPhone;

		std::vector<KeyRect> keyRects;
		std::vector<KeyRect> customRects;
	};

	class GenericSkinFactory : public SkinFactory {
	public:
		DeviceSkin* createInstanceFor(DeviceProfile* dp) const;
		SkinFactory::Fitness getFitnessFor(DeviceProfile* dp) const;
	};
}

#endif // _GENERICSKIN_H_