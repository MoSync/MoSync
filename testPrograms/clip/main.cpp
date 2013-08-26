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
