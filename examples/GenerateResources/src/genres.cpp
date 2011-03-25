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
