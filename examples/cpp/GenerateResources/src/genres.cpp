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
#include <MAUI/Font.h>
#include "MAHeaders.h"

using namespace MAUtil;

class GRMoblet : public Moblet {
public:
	GRMoblet() {
		int y = 0;
		maSetColor(0xFFFFFF);
		MAExtent ss = maGetScrSize();
		maFillRect(0,0, EXTENT_X(ss), EXTENT_Y(ss));

		maDrawImage(RES_COMBO5, 0, 0);
		y += EXTENT_Y(maGetImageSize(RES_COMBO5));

		maDrawImage(RES_COMBO10, 0, y);
		y += EXTENT_Y(maGetImageSize(RES_COMBO10));

		maDrawImage(RES_SPINNER_ANIM, 0, y);
		y += EXTENT_Y(maGetImageSize(RES_SPINNER_ANIM));

		MAUI::Font f(RES_FONT_ARIAL);
		f.drawString("Hello World!", 0, y);
		y += f.getCharset().lineHeight;

		f.setResource(RES_FONT_COURIER);
		f.drawString("Hello World!", 0, y);
		y += f.getCharset().lineHeight;

		maSoundPlay(RES_MOBILESORCERY2, 0, maGetDataSize(RES_MOBILESORCERY2));
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}
};

extern "C" int MAMain() {
	Moblet::run(new GRMoblet());
	return 0;
}
