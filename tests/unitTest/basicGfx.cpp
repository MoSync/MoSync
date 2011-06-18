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

#include <mavsprintf.h>
#include "common.h"
#include "MAHeaders.h"

int drawColorRects();
int plot();
int drawLines();
int drawTriangles();
int drawTrianglesClip();
int drawText();

const char *drawImageRegionEnumStrings[] = {
"TRANS_NONE",
"TRANS_MIRROR_ROT180",
"TRANS_MIRROR",
"TRANS_ROT180",
"TRANS_MIRROR_ROT270",
"TRANS_ROT90",
"TRANS_ROT270",
"TRANS_MIRROR_ROT90"
};

/**
 * @brief Tests the maDrawImageRegion
 *
 * This test tests all the available rotations of maDrawImageRegion
 * and prints which rotation it's currently showing.
 */
class DrawImageRegionCase : public KeyBaseCase {
private:
	MAHandle mImage;
	int mState;
public:
	DrawImageRegionCase(const String& name, MAHandle image, int state) : KeyBaseCase(name), mImage(image), mState(state) {}

	virtual void start()
	{
		MAExtent e = maGetScrSize();
		Dimensions screen;
		screen.width = EXTENT_X(e);
		screen.height = EXTENT_Y(e);

		maSetClipRect(0, 0, screen.width, screen.height);
		maSetColor(BLACK);
		maFillRect(0, 0, screen.width, screen.height);

		MARect rect;
		rect.left = 0;
		rect.top = 0;
		MAExtent imgSize = maGetImageSize(mImage);
		rect.width = EXTENT_X(imgSize);
		rect.height = EXTENT_Y(imgSize);
		MAPoint2d p;
		p.x = 0;
		p.y = 0;

		maDrawImageRegion(mImage, &rect, &p, mState);

		const char* title = drawImageRegionEnumStrings[mState];

		maSetColor(0xff0000);
		maDrawText(4, screen.height - EXTENT_Y(maGetTextSize(title)), title);

		maUpdateScreen();
	}

	//KeyListener
	virtual void keyPressEvent(int keyCode) {
	}

	virtual void keyReleaseEvent(int keyCode) {
		checkYesNo(keyCode);
		suite->runNextCase();
	}
};


template <MAHandle image, int state>
void drawImageRegion() {

}

void addBasicGfxTests(TestSuite* suite);
void addBasicGfxTests(TestSuite* suite) {
	suite->addTestCase(new TemplateCase<drawColorRects>("color"));
	suite->addTestCase(new TemplateCase<plot>("plot"));
	suite->addTestCase(new TemplateCase<drawLines>("lines"));
	suite->addTestCase(new TemplateCase<drawTriangles>("triangles"));
	suite->addTestCase(new TemplateCase<drawTrianglesClip>("triangles-clip"));
	suite->addTestCase(new TemplateCase<drawText>("text"));

	for(int i = 0; i < 8; i++)
		suite->addTestCase(new DrawImageRegionCase("drawImageRegion " + String(drawImageRegionEnumStrings[i]), RES_IMAGETEST, i));
}


int drawText() {
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);

	maSetClipRect(0, 0, screen.width, screen.height);
	maSetColor(BLACK);
	maFillRect(0, 0, screen.width, screen.height);

	maSetColor(WHITE);

	maDrawText(0, 0, "Top left");

	const char* br = "Bottom right";
	e = maGetTextSize(br);
	maDrawText(screen.width - EXTENT_X(e), screen.height - EXTENT_Y(e), br);

	const char* middle = "Middle";
	e = maGetTextSize(middle);
	maDrawText((screen.width - EXTENT_X(e)) / 2, (screen.height - EXTENT_Y(e)) / 2, middle);

	maUpdateScreen();
	return 0;
}


int drawTrianglesClip() {
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);

	maSetClipRect(0, 0, screen.width, screen.height);
	maSetColor(BLACK);
	maFillRect(0, 0, screen.width, screen.height);

	maSetClipRect((screen.width>>2), (screen.height>>2), screen.width-(screen.width>>1), screen.height-(screen.height>>1));
	drawTriangles();

	return 0;
}

int drawTriangles() {
	MAExtent e = maGetScrSize();
	Dimensions screen;
	screen.width = EXTENT_X(e);
	screen.height = EXTENT_Y(e);

	maSetColor(BLACK);
	maFillRect(0, 0, screen.width, screen.height);

#define NUM_POINTS 32
	MAPoint2d points[256];

	int x = screen.width>>1;
	int y = screen.height>>1;

	points[0].x = x;
	points[0].y = y;

	//double time = (double)maGetMilliSecondCount()*0.0002;
	double time = 0;
	double size = (screen.width<screen.height)?(screen.width>>1):(screen.height>>1);
	double ang = 0.0;
	double angdelta = (2*3.14159)/(double)NUM_POINTS;
	int index = 0;

	for(int i = 0; i <= NUM_POINTS; i++) {
		points[index].x = x + (int)(cos(ang+time)*size);
		points[index].y = y + (int)(sin(ang+time)*size);
		ang+=angdelta;
		index++;

		if((index%2) == 0) {
			points[index].x = x;
			points[index].y = y;

			maSetColor(0x444444+i*0x220022);			
			maFillTriangleStrip(points, index+1);

			index = 0;
		}
	}

	maUpdateScreen();
	return 0;
}


int drawLines() {
	const MAExtent scrSize = maGetScrSize();
	const int scrHeight = EXTENT_Y(scrSize);
	const int scrWidth = EXTENT_X(scrSize);

	maSetColor(0x808080);
	maFillRect(0, 0, scrWidth, scrHeight);

	maSetColor(0xffffff);
	maLine(50, 50, 100, 100);
	maUpdateScreen();
	maLine(50, -50, 100, -100);
	maUpdateScreen();
	maLine(-50, 50, -100, 100);
	maUpdateScreen();
	maLine(-50, -50, -100, -100);
	maUpdateScreen();
	maLine(-10, 20, 20, -10);	//5
	maUpdateScreen();
	maLine(-1, 1, 1, -1);	//6-7
	maUpdateScreen();
	maLine(-20, 10, 10, -20);	//8
	maUpdateScreen();
	maLine(-40, 40, 50, -100);	//9
	maUpdateScreen();
	maLine(172, 226, 176, 205);	//crash test dummy
	maUpdateScreen();
	maLine(-5, 1, 2, -1);
	maUpdateScreen();

	return 0;
}

int plot() {
	MAExtent screenSize;
	int scrW, scrH;
	screenSize = maGetScrSize();
	scrW = EXTENT_X(screenSize);
	scrH = EXTENT_Y(screenSize);
	maSetColor(BLACK);
	maFillRect(0, 0, scrW, scrH);

	maSetColor(0xFFFFFF);	//white
	maPlot(0, 0);
	maPlot(0, scrH - 1);
	maPlot(scrW - 1, 0);
	maPlot(scrW - 1, scrH - 1);

	maPlot(scrW/2, scrH/2);


	maUpdateScreen();

	return 0;
}


int drawColorRects() {
	MAExtent screenSize;
	int midX, midY;
	int width, height;
	int scrW, scrH;

	screenSize = maGetScrSize();
	scrW = EXTENT_X(screenSize);
	scrH = EXTENT_Y(screenSize);
	midX = scrW >> 1;
	midY = scrH >> 1;
#ifdef RECT_SIZE_TEST
	width = 1;
	height = 1;
#else	//normal color test
	width = midX - 2;
	height = midY / 2 - 2;
#endif

	maSetColor(0x800080);	//deep purple
	maFillRect(0, 0, scrW, scrH);

	maSetColor(RGB(255, 0, 0));				//RED
	maFillRect(1, 1, width, height);
	maSetColor(RGB(255, 255, 0));				//YELLA
	maFillRect(1+width+2, 1, width, height);
	maSetColor(RGB(0, 255, 0));				//GREEN
	maFillRect(1, 1+height+2, width, height);
	maSetColor(RGB(0, 255, 255));				//TORQUOISE
	maFillRect(1+width+2, 1+height+2, width, height);
	maSetColor(RGB(0, 0, 255));				//BLUE
	maFillRect(1, 1+(height+2)*2, width, height);
	maSetColor(RGB(255, 0, 255));				//PURPLE
	maFillRect(1+width+2, 1+(height+2)*2, width, height);
	maSetColor(RGB(255, 255, 255));				//WHITE
	maFillRect(1, 1+(height+2)*3, width, height);
	maSetColor(BLACK);
	maFillRect(1+width+2, 1+(height+2)*3, width, height);

	maUpdateScreen();

	return 0;
}
