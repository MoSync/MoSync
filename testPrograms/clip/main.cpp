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

using namespace MAUtil;

class Cliplet : public Moblet {
	//have fillRect and clipRect. move them using arrow/numbers.
	//toggle control between them with softLeft.

	//or not
public:
	Cliplet() {
		maSetColor(0x800080);	//dark pink
		maFillRect(0, 0, 150, 150);

		maSetClipRect(25, 25, 25, 25);
		maSetColor(0xff);	//blue
		maFillRect(0, 0, 150, 150);

		maSetClipRect(50, 50, 25, 25);
		maSetColor(0xff00);	//green
		maFillRect(0, 0, 150, 150);
		maSetColor(0xff00ff);	//bright pink
		maLine(0, 0, 78, 75);	//two green pixels at top, three at right, on Java & SDL. one/two on Symbian.

		maPlot(49, 60);	//NOT visible
		maPlot(50, 60);	//visible
		maPlot(51, 60);	//visible

		maPlot(74, 60);	//visible
		maPlot(75, 60);	//NOT visible
		maPlot(76, 60);	//NOT visible

		maLine(0, 70, 80, 70);	//horizontal line draw test

		maLine(80, 72, 0, 72);	//reverse horizontal line draw test

#ifdef TOTAL_LINE_DRAW_TEST
		maSetClipRect(0, 0, 150, 150);
		maLine(0, 0, 78, 75);
		maSetClipRect(50, 50, 150, 150);
#endif

		//polygon
		maSetClipRect(25, 25, 25, 25);
		{
			MAPoint2d points[] = { {30, 30}, {40, 40}, {30, 40} };
			maFillPolygon(points, 3);
		}
		{
			MAPoint2d points[] = { {40, 20}, {55, 20}, {55, 35} };
			maFillPolygon(points, 3);
		}
		{
			MAPoint2d points[] = { {20, 40}, {20, 55}, {35, 55} };
			maFillPolygon(points, 3);
		}

		//TODO: maDrawImage and other advanced functions

		//TODO: check null clip rect (w,h) == (0,0) and negative size

		//TODO: test maGetClipRect

		//TODO: do this entire thing on a drawable image

		maUpdateScreen();
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}
};

extern "C" int MAMain() {
	Moblet::run(new Cliplet());
}
