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

#include <ma.h>
#include <madmath.h>
#include "logo_points.h"

void drawCircle(int *buffer, int width, int height, int col) {
	double len;
	int c;
	int ymul;
	for(int j = 0; j < height; j++) {
		ymul = ((j-(height>>1))*(j-(height>>1)));
		for(int i = 0; i<width; i++) {
			len = (sqrt((double)((i-(width>>1))*(i-(width>>1)))+(double)ymul));	
			c = 255-(int)(len*len*len*2.0);
			if(c<0) c = 0;
			*buffer++ = ((c)<<24)|col;	
		}
	}
}

#define LOGO_Y_OFFSET 30
#define RECT_SIZE 22
void drawMosyncLogo(int w, int h) {
	int i = 0,s;
	int circle[16*16];
	int x = (w>>1)-128 - 8; // remove half the size of the circle
	int y = (h>>1)-128 - 8;
	memset(circle, 0x0, 16*16*sizeof(int));

	maSetColor(0xffffff);
	maFillRect(0, 0, w, h);
	maUpdateScreen();

	drawCircle(circle, 16, 16, 0x00000000);
	MARect srcRect = {0, 0, 16, 16};
	maSetDrawTarget(0);

	// clear the screen to correct color
	maSetColor(0xffffff);
	maFillRect(0, 0, w, h);

	{	//put the text in its proper place
		maSetColor(0);
		//maDrawText(40, 150, "Non-commercial!");	//176x208
		const char* texts[] = { "mosync", "non-commercial"};
		int y = (h>>1) + 46;	//46 = 150 - (208>>1)
		for(size_t i=0; i<sizeof(texts) / sizeof(void*); i++) {
			int textSize = maGetTextSize(texts[i]);
			maDrawText((w - EXTENT_X(textSize)) >> 1, y, texts[i]);
			y += EXTENT_Y(textSize);
		}
	}

	maUpdateScreen();
	//maSleep(500);

	// draw the pretty little box
	i = 0;
	s = RECT_SIZE>>1;
	maSetColor(0xff7e00);
	while(i<s) {
		maFillRect((w>>1)-i, (h>>1)-i - LOGO_Y_OFFSET, i*2, i*2);
		maUpdateScreen();
		maWait(20);
		i++;
	}
	maWait(750);

	// draw the pretty little lines
	for(int j = 0; j <= 3; j+=2) {
		i = j;
		s = 4;
		maSetColor(0);
		while(logo_points[i]) {
			MAPoint2d dstPoint = {x+0+6+logo_points[i], y - LOGO_Y_OFFSET+46+logo_points[i+1]};
			maDrawRGB(&dstPoint, circle, &srcRect, 16);
			maUpdateScreen();
#ifndef MAPIP
			maWait(10);
#endif
			i+=4;
		}

		i = j;
		maSetColor(0);
		while(logo_points[i]) {
			MAPoint2d dstPoint = {x+256-6-logo_points[i], y+256 - LOGO_Y_OFFSET -46-logo_points[i+1]};
			maDrawRGB(&dstPoint, circle, &srcRect, 16);
			maUpdateScreen();
#ifndef MAPIP
			maWait(10);
#endif
			i+=4;
		}

	}
	maWait(500);

	maSetColor(0);
	maFillRect(0, 0, w, h);
	maUpdateScreen();
}

extern "C" {
	int MAMain()
	{
		Extent scrSize = maGetScrSize();
		//for(;;) 
		drawMosyncLogo(EXTENT_X(scrSize), EXTENT_Y(scrSize));
		return 0;
	}
}
