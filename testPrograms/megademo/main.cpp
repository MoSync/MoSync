#include <ma.h>
#include "MAHeaders.h"
#include <MAUtil/FrameBuffer.h>
#include "Demo.h"

#include <conprint.h>
#include "resources/scoopex.h"
#include "PCXLoader.h"

class TestEffect : public Effect {
private:
	PCXImg pcxImage;

public:
	void init() {
		int res;
		if((res=pcxImage.Load(scoopex, sizeof(scoopex)))!=IMG_OK) {
			printf("error code: %d\n", res);
			maPanic(res, "Couldn't load pcx!");
		}
	}

	void prepare() {
		unsigned char *pal = pcxImage.GetPalette();

		for(int i = 0; i < 256; i++) {
			FrameBuffer_setPaletteEntry(i, pal[0], pal[1], pal[2], 0);
			pal+=3;
		}
	}

	void render(Surface* surf, float time) {
		int index = 0;
		int itime = (int)(time*100.0);
		byte* backBuffer = surf->backBuffer;


		int w = pcxImage.GetWidth();
		int h = pcxImage.GetHeight();
		byte* img = pcxImage.GetImg();

		for(int j = 0; j < h; j++) {
			for(int i = 0; i < w; i++) {
				backBuffer[i] = *img++;
			}
			backBuffer += surf->w;
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
