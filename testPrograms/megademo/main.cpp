#include <ma.h>
#include "MAHeaders.h"
#include <MAUtil/FrameBuffer.h>
#include <MAUtil/Vector.h>
#include "Demo.h"

#include <mastdlib.h>
#include <conprint.h>
#include "resources/particle.h"
#include "PCXLoader.h"

class MetaEffect : public Effect {
private:
	PCXImg pcxImage;

	struct MetaBall {
		int x, y;
	};

	MAUtil::Vector<MetaBall> metaBalls;

public:
	void init(Surface* surf) {
		int res;
		if((res=pcxImage.Load(particle, sizeof(particle)))!=IMG_OK) {
			printf("error code: %d\n", res);
			maPanic(res, "Couldn't load pcx!");
		}

		for(int i = 0; i < 10; i++) {
			MetaBall metaBall;
			metaBall.x = rand()%surf->w;
			metaBall.y = rand()%surf->h;
			metaBalls.add(metaBall);
		}
	}

	void prepare() {
		unsigned char *pal = pcxImage.GetPalette();

		for(int i = 0; i < 256; i++) {
			if(i < 128)
				FrameBuffer_setPaletteEntry(i, pal[i*3+0], pal[i*3+1], pal[i*3+2], 0);
			else
				FrameBuffer_setPaletteEntry(i, pal[(255-i)*3+0], pal[(255-i)*3+1], pal[(255-i)*3+2], 0);
		}
	}

	void render(Surface* surf, float time) {
		int index = 0;

		Surface image = {pcxImage.GetWidth(), pcxImage.GetHeight(), pcxImage.GetImg()};

		for(int i = 0; i < metaBalls.size(); i++) {
			double x = (surf->w/2.0) + cos(time + i*0.1)*surf->w/2.0;
			double y = (surf->h/2.0) + sin(time + i*0.1)*surf->h/2.0;

			surf->blit(&image, (int)x, (int)y);
		//	drawMetaBall(surf, surf->w/2, surf->h/2);

		}
	}
};

void cleanup() {
	//maSoundStop();
	FrameBuffer_close();
}

extern "C" int MAMain() {

	int scrWidth = EXTENT_X(maGetScrSize());
	int scrHeight = EXTENT_Y(maGetScrSize());
	byte *backBuffer = new byte[scrWidth*scrHeight];
	FrameBuffer_init(scrWidth, scrHeight, 0, 0);
	Surface surface = {scrWidth, scrHeight, backBuffer};

	addEffect( new MetaEffect() );
	initEffects(&surface);

	//maSoundPlay(R_MUSIC,  0, maGetDataSize(R_MUSIC));

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
