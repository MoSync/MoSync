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

#include <ma.h>
#include <maheap.h>
#include <mavsprintf.h>
#include "MAHeaders.h"

//options

//#define GRADIENT
#define ANYTHING
#define HANDLE_CLOSE_EVENT	//comment out for runtime stability testing


#define DIRECTION_KEYS(m) m(UP) m(LEFT) m(DOWN) m(RIGHT)
#define NUMBER_KEYS(m) m(0) m(1) m(2) m(3) m(4)	m(5) m(6) m(7) m(8) m(9)
#define OTHER_KEYS(m) m(STAR) m(POUND) m(FIRE) m(SOFTLEFT) m(SOFTRIGHT) m(CLEAR)
#define ALL_KEYS(m) DIRECTION_KEYS(m) NUMBER_KEYS(m) OTHER_KEYS(m)

#define E_COMMA(m) E_##m,
enum UsedKey {
	ALL_KEYS(E_COMMA)	N_USED_KEYS
};

#define BACKGROUND_COLOR backgroundColor//0x700000

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))
void DrawCardinalArrow(int baseX, int baseY, int length, int mPX, int mPY);
void DrawCenteredText(int top, const char* text);
void DrawRightAlignedText(int top, const char* text);

int backgroundColor = 0x700000;

void myDrawText(int left, int top, const char* text) {
	maSetColor(-1);
	maDrawText(left, top, text);
}

extern "C" int MAMain() {
	int baseMSC = maGetMilliSecondCount();
#ifdef ANYTHING
	int time = 0;
	int x=0, y=0, dx=1, dy=1;
	int textHeight = EXTENT_Y(maGetTextSize(" "));
#ifdef GRADIENT
	int i;
#endif
#endif
	BOOL keys[N_USED_KEYS] = { false };
	MAExtent imgSize;

	int frameCount = 0;
	int lastSecondMSC = baseMSC;
	int lastSecondFrameCount = 0;
	int fpsDelay = -1, fpsImm = -1;

	imgSize = maGetImageSize(RES_IMAGE);
	while(1) {
		MAExtent screenSize;
		int midX, midY;
		int length;
		char buffer[64];
		int currentMSC = maGetMilliSecondCount();
#ifdef ANYTHING
		int deltaTime;
#endif

		{
			MAEvent event;
			if(maGetEvent(&event)) {
				if(
#ifdef HANDLE_CLOSE_EVENT
					event.type == EVENT_TYPE_CLOSE ||
#endif
					(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
				{
					maExit(0);
				} else if(event.type == EVENT_TYPE_KEY_PRESSED) {
					switch(event.key) {
#define HANDLE_KEY_PRESSED(key) case MAK_##key: keys[E_##key] = TRUE; break;
						ALL_KEYS(HANDLE_KEY_PRESSED);
					}
				} else if(event.type == EVENT_TYPE_KEY_RELEASED) {
					switch(event.key) {
#define HANDLE_KEY_RELEASED(key) case MAK_##key: keys[E_##key] = FALSE; break;
						ALL_KEYS(HANDLE_KEY_RELEASED);
					}
				} else if(event.type == EVENT_TYPE_POINTER_PRESSED) {
					keys[E_FIRE] = TRUE;
				} else if(event.type == EVENT_TYPE_POINTER_RELEASED) {
					keys[E_FIRE] = FALSE;
				}
			}
		}

		screenSize = maGetScrSize();
		midX = EXTENT_X(screenSize) >> 1;
		midY = EXTENT_Y(screenSize) >> 1;
		length = MIN(midX, midY) >> 1;

		sprintf(buffer, "%i", frameCount);
		myDrawText(midX+midX/2, 0, buffer);

		frameCount++;
		if(currentMSC >= lastSecondMSC + 1000) {
			fpsDelay = frameCount - lastSecondFrameCount;
			lastSecondFrameCount = frameCount;
			lastSecondMSC = currentMSC;
		}
		if(fpsDelay != -1) {
			sprintf(buffer, "%i FPSd", fpsDelay);
			myDrawText(0,0, buffer);
		}
		if(fpsImm != -1) {
			sprintf(buffer, "%i FPSi", fpsImm);
			myDrawText(0,0, buffer);
		}

#ifdef ANYTHING
		//  time++;
		deltaTime = time;	//oldtime
		time = (currentMSC - baseMSC);// / 20;
		deltaTime = time - deltaTime;
		//  if(time >= 1536) time -= 1536;
		sprintf(buffer, "t %i dt %i", time, deltaTime);
		myDrawText(midX-midX/2, textHeight*1, buffer);

		DrawCenteredText(textHeight*2, "Graphics/input test");
		DrawRightAlignedText(textHeight*3, __DATE__ " " __TIME__);

#ifdef GRADIENT
		//Gradient
		maSetColor(-1);
		for(i=0; i<128; i++) {
			/*int cycle = ((frameCount+i*8) / 256) % 6, subtime = (frameCount+i*8) % 256;
			switch(cycle) {
			case 0:	maSetColor(RGB(255, subtime, 0));		break;
			case 1:	maSetColor(RGB(255-subtime, 255, 0));	break;
			case 2:	maSetColor(RGB(0, 255, subtime));		break;
			case 3:	maSetColor(RGB(0, 255-subtime, 255));	break;
			case 4:	maSetColor(RGB(subtime, 0, 255));		break;
			case 5:	maSetColor(RGB(255, 0, 255-subtime));	break;
			}*/
			//maFillRect(midX/8, i+midX/2, midX/3, 1);
			maLine(midX/8, i+midX/2, midX/3, i+midX/2);

//			if(i == 0)	//make the color darker
//				backgroundColor = (maSetColor(0) >> 1) & ~0x808080;
			backgroundColor = 0;
		}
#else
		{
			int cycle = (frameCount / 256) % 6, subtime = frameCount % 256;
			switch(cycle) {
			case 0:	maSetColor(RGB(255, subtime, 0));		break;
			case 1:	maSetColor(RGB(255-subtime, 255, 0));	break;
			case 2:	maSetColor(RGB(0, 255, subtime));		break;
			case 3:	maSetColor(RGB(0, 255-subtime, 255));	break;
			case 4:	maSetColor(RGB(subtime, 0, 255));		break;
			case 5:	maSetColor(RGB(255, 0, 255-subtime));	break;
			}
			backgroundColor = (maSetColor(0) >> 1) & ~0x808080;
		}
#endif	//GRADIENT

		maDrawImage(RES_IMAGE, x, y);
		if((dx > 0 && x + EXTENT_X(imgSize) > EXTENT_X(screenSize)) || (dx < 0 && x < 0)) {
			dx = -dx;
		}
		if((dy > 0 && y + EXTENT_Y(imgSize) > EXTENT_Y(screenSize)) || (dy < 0 && y < 0)) {
			dy = -dy;
		}
		x += dx;// * deltaTime;
		y += dy;// * deltaTime;

		{
			char buf[16];
			char *ptr = buf;
			//if(keys & MAK_0) *ptr++ = '0';
#define HANDLE_NUMBER_KEY(n) if(keys[E_##n]) *ptr++ = ('0' + n);
			NUMBER_KEYS(HANDLE_NUMBER_KEY);
			if(keys[E_POUND]) *ptr++ = '#';
			if(keys[E_STAR]) *ptr++ = '*';
			if(keys[E_SOFTLEFT]) *ptr++ = 'L';
			if(keys[E_SOFTRIGHT]) *ptr++ = 'R';
			if(keys[E_CLEAR]) *ptr++ = 'C';

			if(ptr != buf) {
				*ptr = 0;
				DrawCenteredText(textHeight*4, buf);
			}
		}

		if(keys[E_FIRE]) {
			DrawCardinalArrow(midX, midY, length, 0, 1);
			DrawCardinalArrow(midX, midY, length, 0, -1);
			DrawCardinalArrow(midX, midY, length, 1, 1);
			DrawCardinalArrow(midX, midY, length, 1, -1);
			DrawCardinalArrow(midX, midY, length, 1, 0);
			DrawCardinalArrow(midX, midY, length, -1, 0);
			DrawCardinalArrow(midX, midY, length, -1, 1);
			DrawCardinalArrow(midX, midY, length, -1, -1);
		}

		{
			int px=0, py=0;
			if(keys[E_LEFT])
				px -= 1;
			if(keys[E_RIGHT])
				px += 1;
			if(keys[E_DOWN])
				py += 1;
			if(keys[E_UP])
				py -= 1;
			if(px || py)
				DrawCardinalArrow(midX, midY, length, px, py);
		}
#endif	//ANYTHING
		maUpdateScreen();
		maSetColor(BACKGROUND_COLOR);
		maFillRect(0, 0, EXTENT_X(screenSize), EXTENT_Y(screenSize));
	}
}

void DrawCenteredText(int top, const char* text) {
	//MAExtent textSize = maGetTextSize(text);
	//myDrawText((EXTENT_X(maGetScrSize()) - EXTENT_X(textSize)) / 2, top, text);
	myDrawText(0, top, text);
}
void DrawRightAlignedText(int top, const char* text) {
	//MAExtent textSize = maGetTextSize(text);
	//myDrawText((EXTENT_X(maGetScrSize()) - EXTENT_X(textSize)), top, text);
	myDrawText((EXTENT_X(maGetScrSize()) >> 2), top, text);
}

void DrawCardinalArrow2(int baseX, int baseY, int length, int mPX, int mPY) {
	int pointX = baseX + (length * mPX);
	int pointY = baseY + (length * mPY);
	int len3rd = length / 3;
	maSetColor(0xFFFF0000);
	maLine(baseX, baseY, pointX, pointY);
	maSetColor(0xFF00FF00);
	maLine(pointX, pointY, pointX + (len3rd * (mPX*-1 + mPY*-1)),
		pointY + (len3rd * (mPX*1 + mPY*-1)));
	maSetColor(0xFF0000FF);
	maLine(pointX, pointY, pointX + (len3rd * (mPX*-1 + mPY*1)),
		pointY + (len3rd * (mPX*-1 + mPY*-1)));
}

void DrawCardinalArrow(int baseX, int baseY, int length, int mPX, int mPY) {
	DrawCardinalArrow2(baseX, baseY, length, mPX, mPY);
	if(EXTENT_X(maGetScrSize()) > 300)
		DrawCardinalArrow2(baseX+mPY, baseY, length, mPX, mPY); //imperfect
}

