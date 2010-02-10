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

/** \file main.cpp
* MoSketch is an Etch-A-Sketch analogue written in MoSync.
* The background is gray. The pointer is white. Use the arrow keys to move the pointer
* and draw a black polyline. Press 9 to clear the screen.
* Press 2 to "lift" the pointer, or to put it back down.
* Press 7 to have the program draw the MoSync logotype. //DISABLED!
* Press 0 (zero) to exit the program.
* The sketch is saved to a permanent store automatically when you exit the program.
* If a sketch has been saved, it will be loaded automatically when you start the program.
*/

#include <ma.h>
#include <conprint.h>
#include "MAHeaders.h"
#include <maassert.h>
#include <MAUtil/DataHandler.h>
#include "MoSketch.h"

#define GRAY 0xA0A0A0//0xffffff//
#define WHITE 0xffffff
#define BLACK 0

#define TEST(func) if(!(func)) return false;

MAPoint2d gPointer, gOldD, gD, gScreenSize;
Vector<PolyLine> gPolyLines;
PolyLine gCurrentPolyLine;
int gOldKeys = 0;
bool gPenDown = true;

void drawPtr();
void movePtr(MAPoint2d& d);
//void doMenu();




bool loadPolyLine(DataHandler& data, PolyLine& pl) {
	int length;
	TEST(data.read(&length, sizeof(length)));
	if(length <= 0)
		return false;
	pl.resize(length);
	TEST(data.read(pl.pointer(), length * sizeof(ShortPoint)));

	//animate-draw the loaded sketch
	ShortPoint p = pl[0];
	gPointer.x = p.x;
	gPointer.y = p.y;
	for(int j=1; j<length; j++) {
		p = pl[j];
		MAPoint2d d = { p.x - gPointer.x, p.y - gPointer.y };
		movePtr(d);
		drawPtr();
		//maSleep(10);
	}
	return true;
}

//Tries to load a sketch from a data object. Returns false iff the save file is corrupt.
bool loadFromData(MAHandle object) {
	DataHandler data(object);
	int nPolyLines;
	TEST(data.read(&nPolyLines, sizeof(nPolyLines)));

	maSetColor(GRAY);
	maFillRect(0, 0, gScreenSize.x, gScreenSize.y);

	gPolyLines.clear();
	if(nPolyLines > 0) {
		gPenDown = true;
		gPolyLines.resize(nPolyLines - 1);
		for(int i=0; i<gPolyLines.size(); i++) {
			TEST(loadPolyLine(data, gPolyLines[i]));
		}
		TEST(loadPolyLine(data, gCurrentPolyLine));
	}
	TEST(data.read(&gPenDown, sizeof(gPenDown)));
	ShortPoint p;
	TEST(data.read(&p, sizeof(ShortPoint)));
	gPointer.x = p.x;
	gPointer.y = p.y;
	drawPtr();

	return true;
}

//clears the sketch and draws the MoSync logotype.
void drawLogo() {
#ifdef DRAW_LOGO
	MYASSERT(loadFromData(RES_LOGO));
#endif
}

//Tries to load a sketch from the Store. Returns false iff the save file is corrupt.
bool tryLoad() {
	MAHandle store = maOpenStore("MoSketch.sav", 0);
	if(store <= 0) {
		drawLogo();
		return true;
	}
	maDestroyObject(RES_STORE);
	maReadStore(store, RES_STORE);
	maCloseStore(store, false);

	return loadFromData(RES_STORE);
}

//Saves the current sketch. Returns false on failure.
bool save() {
	//open store
	MAHandle store = maOpenStore("MoSketch.sav", MAS_CREATE_IF_NECESSARY);
	if(store <= 0)
		return false;

	//calculate size
	int nPolyLines = gPolyLines.size();
	int sizeofPolyLines = 0;
	for(int i=0; i<nPolyLines; i++) {
		MAASSERT(gPolyLines[i].size() > 0);
		sizeofPolyLines += sizeof(int) + gPolyLines[i].size() * sizeof(ShortPoint);
	}
	int fullSize = sizeof(nPolyLines) + sizeofPolyLines + sizeof(gPenDown) + sizeof(ShortPoint);

	//create data object
	maDestroyObject(RES_STORE);	//destroy previous data object, if any
	maCreateData(RES_STORE, fullSize);

	//write data
	DataHandler data(RES_STORE);
	data.write(&nPolyLines, sizeof(nPolyLines));
	for(int i=0; i<nPolyLines; i++) {
		const PolyLine& pl(gPolyLines[i]);
		int length = pl.size();
		data.write(&length, sizeof(length));
		data.write(pl.pointer(), length * sizeof(ShortPoint));
	}
	data.write(&gPenDown, sizeof(gPenDown));
	ShortPoint p = { gPointer.x, gPointer.y };
	data.write(&p, sizeof(ShortPoint));

	//write and close store
	if(maWriteStore(store, RES_STORE) <= 0)
		return false;
	maCloseStore(store, false);
	return true;
}

//Add the current pointer position as a point in the sketch polyline.
void addPoint() {
	if(gPenDown) {
		ShortPoint p = { gPointer.x, gPointer.y };
		gCurrentPolyLine.add(p);
//	lprintfln("vadd %i %ix%i", gVector.size(), gPointer.x, gPointer.y);
	}
}

//clears the sketch
void clear() {
	gCurrentPolyLine.clear();
	gPolyLines.clear();

	gD.x = gD.y = gOldD.x = gOldD.y = 0;

	maSetColor(GRAY);
	maFillRect(0, 0, gScreenSize.x, gScreenSize.y);
	drawPtr();

	addPoint();
}

//Moves the pointer by a delta value.
//Draws a line from the old point to the new.
//Changes the argument to reflect the actual movement of the pointer,
//if the delta would move the pointer outside the screen.
void movePtr(MAPoint2d& d) {
	if(gPointer.x + d.x >= gScreenSize.x || gPointer.x + d.x < 0)
		d.x = 0;
	if(gPointer.y + d.y >= gScreenSize.y || gPointer.y + d.y < 0)
		d.y = 0;
	if(!d.x && !d.y)
		return;
	MAPoint2d newP = { gPointer.x + d.x, gPointer.y + d.y };
	if(gPenDown) {
		maSetColor(BLACK);
		maLine(gPointer.x, gPointer.y, newP.x, newP.y);
	}
	gPointer = newP;
}

void drawPtr() {
	maSetDrawTarget(HANDLE_SCREEN);
	maDrawImage(RES_BACKBUFFER, 0, 0);
	maSetColor(WHITE);
	maPlot(gPointer.x, gPointer.y);
	maUpdateScreen();
	maSetDrawTarget(RES_BACKBUFFER);
}

int currentKeyState = 0;

#define LEFT	0x0001
#define RIGHT	0x0002
#define UP		0x0004
#define DOWN	0x0008
#define FIRE	0x0010

int getKeyFlag(int key) {
	switch(key) {
		case MAK_LEFT: return LEFT;
		case MAK_RIGHT: return RIGHT;
		case MAK_UP: return UP;
		case MAK_DOWN: return DOWN;
		case MAK_FIRE: return FIRE;
		default: return 0;
	}
}

int getKeys() {
	return currentKeyState;
}

extern "C" int MAMain() GCCATTRIB(noreturn);
extern "C" int MAMain() {
	//init
	int screenSize = maGetScrSize();
	gScreenSize.x = EXTENT_X(screenSize);
	gScreenSize.y = EXTENT_Y(screenSize);

	maCreateDrawableImage(RES_BACKBUFFER, gScreenSize.x, gScreenSize.y);
	maSetDrawTarget(RES_BACKBUFFER);

	gPointer.x = gScreenSize.x / 2;
	gPointer.y = gScreenSize.y / 2;
	clear();

	if(!tryLoad())
		clear();

	//main loop
	while(true) {
		while(true) {
			MAEvent event;
			//maWait(0);
			while(maGetEvent(&event)) {
				if(event.type == EVENT_TYPE_CLOSE ||
					(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
				{
					//save the last point
					ShortPoint lastP = gCurrentPolyLine[gCurrentPolyLine.size() - 1];
					if(!(lastP.x == gPointer.x && lastP.y == gPointer.y)) {
						addPoint();
					}

					//save the last line
					if(gCurrentPolyLine.size() > 1) {
						gPolyLines.add(gCurrentPolyLine);
					}

					save();	//save the sketch to permanent store
					maExit(0);
				}
				if(event.type == EVENT_TYPE_KEY_PRESSED) {
					currentKeyState |= getKeyFlag(event.key);
					if(event.key == MAK_2) {	//Lift / put down
						if(gPenDown) {
							gPenDown = false;
							if(gCurrentPolyLine.size() > 1) {
								gPolyLines.add(gCurrentPolyLine);
							}
						} else {
							gPenDown = true;
							gCurrentPolyLine.clear();
							addPoint();
						}
					}
					if(event.key == MAK_9) {	//clear
						clear();
					}
					if(event.key == MAK_7) {	//Draw logo
						drawLogo();
					}
				}
				else if(event.type == EVENT_TYPE_KEY_RELEASED) {
					currentKeyState &= ~getKeyFlag(event.key);
				}
			}

			if(currentKeyState != gOldKeys) {
				gOldKeys = currentKeyState;
				break;
			}
			maWait(20);
			if(gD.x || gD.y) {
				MAPoint2d oldP = gPointer;

				movePtr(gD);
				drawPtr();

				//hack to handle diagonals and screen edges
				if((gD.x != gOldD.x || gD.y != gOldD.y) && (gOldD.x || gOldD.y)) {
					MAPoint2d newP = gPointer;
					gPointer = oldP;
					addPoint();
					gPointer = newP;
					gOldD = gD;
				}
			}
		}

		gD.x = gD.y = 0;
		if(currentKeyState & LEFT)
			gD.x -= 1;
		if(currentKeyState & RIGHT)
			gD.x += 1;
		if(currentKeyState & UP)
			gD.y -= 1;
		if(currentKeyState & DOWN)
			gD.y += 1;

		if(gD.x || gD.y) {	//there's movement
			if((gD.x != gOldD.x || gD.y != gOldD.y) && (gOldD.x || gOldD.y)) {	//old line ends
				addPoint();
			}
			movePtr(gD);
			drawPtr();
			gOldD = gD;
		}
	}
}

void doMenu() {
	//exit menu
	//save
	//load
	//clear
	//exit program
}
