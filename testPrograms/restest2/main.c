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
#include <maheap.h>
#include <mavsprintf.h>

#include "maheaders.h"

int abs(int a) {
	return (a) > 0 ? a : -a;
}

typedef struct IMGPOS {
	int x, y, dx, dy, w, h;
} IMGPOS;

IMGPOS p[N_IMAGES];

int scrSize;
int scrWidth;
int scrHeight;

int resSet = 1;
//static const int nResSets = 2;

char textBuffer[64];
const char* displayedText[2] = {"",""};


void InitText();
void DisplaySecondaryText(const char* text);


int MAMain() {
	int i;
	//int soundLength;
	//int musicLength;
	enum State { Idle, Waiting, Fetching } state = Idle;
	scrSize = maGetScrSize();
	scrWidth = EXTENT_X(scrSize);
	scrHeight = EXTENT_Y(scrSize);

	InitText();
	displayedText[0] = textBuffer;

#define CP p[i-1]

	//IMGPOS init
	for(i=1; i<=N_IMAGES; i++) {
		int imgSize;
		CP.x = abs(maRand() % scrWidth);
		CP.y = abs(maRand() % scrHeight);
		CP.dx = 1;
		CP.dy = 1;
		imgSize = maGetImageSize(i);
		CP.w = EXTENT_X(imgSize);
		CP.h = EXTENT_Y(imgSize);
	}

	//Sound init
	//maSoundInit(0, R_SOUND);
	//maSoundInit(1, R_SOUND);//R_MUSIC, 1);//
/*	do {
		soundLength = maSoundGetLength(0);
		maGetKeys();
	} while(soundLength <= 0);
	do {
		musicLength = maSoundGetLength(1);
		maGetKeys();
	} while(musicLength <= 0);*/
	//maSoundStart(1);

	while(1) {
		int i, key;//, then = maGetMilliSecondCount();
		maSetColor(0x00007f);	//blue
		maFillRect(0, 0, scrWidth, scrHeight);
		for(i=1; i<=N_IMAGES; i++) {
			maDrawImage(i, CP.x, CP.y);
			if((CP.dx > 0 && CP.x + CP.w > scrWidth) || (CP.dx < 0 && CP.x < 0)) {
				CP.dx = -CP.dx;
			}
			if((CP.dy > 0 && CP.y + CP.h > scrHeight) || (CP.dy < 0 && CP.y < 0)) {
				CP.dy = -CP.dy;
			}
			CP.x += CP.dx;
			CP.y += CP.dy;
		}
		key = maGetBufferedKeyEvent();
		if(key) {
			static char buffer[32];
			sprintf(buffer, "Last key event: 0x%x", key);
			displayedText[1] = buffer;
		}
		maDrawText(0,0, displayedText[0]);
		maDrawText(0,16, displayedText[1]);
		/*{
			char buffer[32];
			sprintf(buffer, "%i ms", soundLength);
			maDrawText(0,32, buffer);
		}*/
		maUpdateScreen();

		switch(state) {
		case Idle:
			if(key == MAK_FIRE) {	//reload resources
				state = Waiting;
//				maSoundSetPosition(0, 0);
//				maSoundStop(0);
//				maSoundSetLoopCount(0, 2);
//				maSoundStart(0);
			}
			break;
		case Waiting:
			if(key != MAK_FIRE) {
				static char loading[32] = "Loading X...";
				static char url[] = "wap.mopix.se/test/resX";
				//"nonexistant.example.com/resX";	//test of HTTP system's DNS error reporting caps.
				loading[8] = '0' + resSet;
				DisplaySecondaryText(loading);

				url[sizeof(url)-2] = '0' + resSet;
				maHttpGetToObject(url, DATA);
				state = Fetching;
			}
			break;
		case Fetching:
			if(maConnState()) {
				BOOL failure = maConnState() < 0;
				state = Idle;
				if(!failure) {
					failure = !maLoadResources(DATA);
					maDestroyObject(DATA);
				}
				if(failure) {
					static char buffer[32];
					sprintf(buffer, "Loading %i failed, %i", resSet, maConnState());
					DisplaySecondaryText(buffer);
				} else {
					int i;
					for(i=1; i<=N_IMAGES; i++) {
						int imgSize = maGetImageSize(i);
						CP.w = EXTENT_X(imgSize);
						CP.h = EXTENT_Y(imgSize);
					}
					InitText();
					resSet++;
					displayedText[1] = "";
				}
			}
		}
		/*{
		int now = maGetMilliSecondCount();
		int time_padding = 10 - (now - then);
		if(time_padding > 0)
		maSleep(time_padding);
		}*/
	}
}

void InitText() {
	int size = maGetDataSize(RESNAME);
	if(size >= (int)sizeof(textBuffer))
		maExit(-1);
	maReadData(RESNAME, textBuffer, 0, size);
	textBuffer[size] = 0;
}

void DisplaySecondaryText(const char* text) {
	maFillRect(0, 16, scrWidth, 16);
	maDrawText(0,16, text);
	maUpdateScreen();
	displayedText[1] = text;
}
