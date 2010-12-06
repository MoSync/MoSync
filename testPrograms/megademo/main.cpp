/* Copyright (C) 2010 MoSync AB

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

		for(int y = 0; y < pcxImage.GetHeight(); y++) {
			for(int x = 0; x < pcxImage.GetWidth(); x++) {
				pcxImage.GetImg()[x+y*pcxImage.GetWidth()] >>= 2;
			}
		}

		for(int i = 0; i < 10; i++) {
			MetaBall metaBall;
			metaBall.x = rand()%surf->w;
			metaBall.y = rand()%surf->h;
			metaBalls.add(metaBall);
		}
	}

	void updatePalette(double f) {
		if(f > 2.0) return;

		for(int i = 0; i < 256; i++) {
			double angle = i*f*3.14159/256.0;
			double func = (double)i/32.0; //(cos(angle));
			func *= func;
			int color = (int)(func*255.0);
			if(color<0) color = 0;
			if(color>255) color = 255;


			FrameBuffer_setPaletteEntry(i, color, color, color, 0);

		}
	}

	void prepare() {
		unsigned char *pal = pcxImage.GetPalette();


	}

	void render(Surface* surf, float time) {
		int index = 0;

		double f = time * 0.1;
		if(f>2.0) f = 2.0;
		updatePalette(f);

		Surface image = {pcxImage.GetWidth(), pcxImage.GetHeight(), pcxImage.GetImg()};

		memset(surf->pixels, 0, surf->w*surf->h);

		for(int i = 0; i < metaBalls.size(); i++) {
			double x = (surf->w/2.0) + cos(f*time*0.54 + i*0.8)*surf->w/2.0;
			double y = (surf->h/2.0) + sin(f*time*0.42 + i*1.4)*surf->h/2.0;

			surf->blit(&image, (int)x, (int)y, BLIT_ADD);
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
