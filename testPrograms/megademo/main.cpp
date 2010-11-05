#include <ma.h>
#include "MAHeaders.h"
#include <MAUtil/FrameBuffer.h>
#include "Demo.h"

class TestEffect : public Effect {

	void init() {
	}

	void prepare() {
		for(int i = 0; i < 256; i++) {
			FrameBuffer_setPaletteEntry(i, i, i, i, 0);
		}
	}

	void render(Surface* surf, float time) {
		int index = 0;
		int itime = (int)(time*100.0);

		byte* backBuffer = surf->backBuffer;
		for(int j = 0; j < surf->w; j++) {
			for(int i = 0; i < surf->h; i++) {
				backBuffer[index] = (itime+i)*j;
				index++;
			}
		}
	}
};

void cleanup() {
	maSoundStop();
	FrameBuffer_close();
}

extern "C" int MAMain() {

	int scrWidth = EXTENT_X(maGetScrSize());
	int scrHeight = EXTENT_Y(maGetScrSize());
	byte *backBuffer = new byte[scrWidth*scrHeight];
	FrameBuffer_init(scrWidth, scrHeight, 0, 0);
	Surface surface = {scrWidth, scrHeight, backBuffer};

	addEffect( new TestEffect() );
	initEffects();

	maSoundPlay(R_MUSIC,  0, maGetDataSize(R_MUSIC));

	while(1) {
		MAEvent event;
		while(maGetEvent(&event)) {
			switch(event.type) {
			case EVENT_TYPE_CLOSE:
				cleanup();
				maExit(0);

			case EVENT_TYPE_KEY_PRESSED:
				switch(event.key) {
				case MAK_LEFT: setEffectIndex(getEffectIndex()-1); break;
				case MAK_RIGHT: setEffectIndex(getEffectIndex()+1); break;
				}
			}
		}

		renderEffect(&surface);
		FrameBuffer_copyRect(0, 0, scrWidth, scrHeight, 0, 0, backBuffer, scrWidth);
		maUpdateScreen();
	}

	return 0;
}
