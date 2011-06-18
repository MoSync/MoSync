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

#include <MAUtil/Moblet.h>
#include <MAUtil/Vector.h>

#define CONFIG_SAVE 0

#define COLOR_PICKER_HEIGHT 16
#define NUM_COLORS 8
int colors[NUM_COLORS] = {
	0xff0000,
	0xffff00,
	0x00ff00,
	0x00ffff,
	0x0000ff,
	0xff00ff,
	0x000000,
	0xffffff,
};

using namespace MAUtil;

struct Point {
	short x, y;
};

#if CONFIG_SAVE
struct Segment {
	Vector<Point> points;
	int color;
};

Vector<Segment> segments;
#endif

class MyMoblet : public Moblet {
private:
	int width, height;
	int currentColor;
	Point lastPos;

public:
	MyMoblet() {
		MAExtent s = maGetScrSize();
		width = EXTENT_X(s);
		height = EXTENT_Y(s);
		currentColor = 0xffffff;
		maSetColor(0);
		maFillRect(0, 0, width, height);
		drawPalette();
#if CONFIG_SAVE
		loadImage();
#endif
		maUpdateScreen();
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_SOFTLEFT) {
#if CONFIG_SAVE
			segments.resize(0);
			drawImage();
#else
			//clear screen
			maSetColor(0);
			maFillRect(0, 0, width, height);
			drawPalette();
#endif
		} else {
			closeEvent();
		}
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
	}

#if CONFIG_SAVE
	void drawImage() {
		maSetColor(0);
		maFillRect(0, 0, width, height);
		drawPalette();
		for(int i = 0; i < segments.size(); i++) {
			lastPos = segments[i].points[0];
			for(int j = 1; j < segments[i].points.size(); j++) {
				drawLine(segments[i].points[j].x, segments[i].points[j].y, segments[i].color);
			}
		}
	}

	bool loadImage() {
		MAHandle store = maOpenStore("drawing", 0);
		if(store <= 0) {
			return false;
		}
		MAHandle RES_STORE = maCreatePlaceholder();
		maReadStore(store, RES_STORE);
		maCloseStore(store, false);

#define READ(x, type) type x; maReadData(RES_STORE, &x, dataPos, sizeof(type)); dataPos+=sizeof(type);

		int dataPos = 0;

		READ(numSegments, int);
		segments.resize(numSegments);
		for(int i = 0; i < numSegments; i++) {

			READ(color, int);
			READ(numPoints, int);

			segments[i].color = color;
			segments[i].points.resize(numPoints);
			for(int j = 0; j < numPoints; j++) {
				READ(x, short);
				READ(y, short);
				segments[i].points[j].x = x;
				segments[i].points[j].y = y;
			}
		}

		maDestroyObject(RES_STORE);

		drawImage();
		return true;
	}

	bool saveImage() {
		if(!segments.size()) return true;

		MAHandle store = maOpenStore("drawing", MAS_CREATE_IF_NECESSARY);
		if(store <= 0) {
			return false;
		}

		int totalDataSize = sizeof(int); // number of segments;
		for(int i = 0; i < segments.size(); i++) {
			totalDataSize+=sizeof(int); // color;
			totalDataSize+=sizeof(int); // number of points;
			totalDataSize+=segments[i].points.size()*sizeof(short)*2; // size of points;
		}

		MAHandle RES_STORE = maCreatePlaceholder();
		maCreateData(RES_STORE, totalDataSize);


#define WRITE(x, type) {type temp = x; maWriteData(RES_STORE, &temp, dataPos, sizeof(type)); dataPos+=sizeof(type);}

		int dataPos = 0;

		WRITE(segments.size(), int);
		for(int i = 0; i < segments.size(); i++) {
			WRITE(segments[i].color, int);
			WRITE(segments[i].points.size(), int);
			for(int j = 0; j < segments[i].points.size(); j++) {
				WRITE(segments[i].points[j].x, short);
				WRITE(segments[i].points[j].y, short);
			}
		}

		if(maWriteStore(store, RES_STORE) <= 0)
			return false;
		maCloseStore(store, false);

		return true;
	}
#endif

	void closeEvent() GCCATTRIB(noreturn) {
#if CONFIG_SAVE
		if(saveImage()==false) maPanic(0, "failed to save image!");
#endif

		Moblet::closeEvent();
	}

	void drawPalette() {
		int size = width/NUM_COLORS;
		int index = 0;
		for(int i = 0; i < width; i+= size) {
			maSetColor(colors[index++]);
			maFillRect(i, 0, size, COLOR_PICKER_HEIGHT);
		}
	}

	void pickColor(int x) {
		int index = (x*NUM_COLORS)/width;
		if(index<0) index = 0;
		if(index>=NUM_COLORS) index = NUM_COLORS-1;
		currentColor = colors[index];
	}

	void drawLine(int x, int y, int col) {
		maSetColor(col);
		maLine(lastPos.x, lastPos.y, x, y);
		maUpdateScreen();
		lastPos.x = x;
		lastPos.y = y;
	}

	void pointerPressEvent(MAPoint2d p) {
		if(p.y<COLOR_PICKER_HEIGHT) {
			pickColor(p.x);
			lastPos.x = -1;
			lastPos.y = -1;
		} else {
			lastPos.x = p.x;
			lastPos.y = p.y;
#if CONFIG_SAVE
			segments.resize(segments.size()+1);
			segments[segments.size()-1].color = currentColor;
			segments[segments.size()-1].points.add(lastPos);
#endif
		}
	}

	void pointerMoveEvent(MAPoint2d p) {
		if(lastPos.x == -1 || p.y < COLOR_PICKER_HEIGHT);
		else {
			drawLine(p.x, p.y, currentColor);
#if CONFIG_SAVE
			segments[segments.size()-1].points.add(lastPos);
#endif
		}
	}

	void pointerReleaseEvent(MAPoint2d p) {
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
