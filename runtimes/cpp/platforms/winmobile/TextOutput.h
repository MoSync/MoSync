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

#ifndef _TEXTOUTPUT_H_
#define _TEXTOUTPUT_H_

#include "Image.h"
#include <windows.h>

class TextOutput {
public:
	TextOutput();
	~TextOutput();

	void drawText(Image *dst, int x, int y, const char *str, unsigned int color);
	void drawChar(Image *dst, int x, int y, const char c, unsigned int color);
	SIZE getTextSize(const char *str);
	bool init();
	
private:
	Rect fontCoords[128];
	int spacing;
	unsigned char *fontBitmap;
};

#endif /* _TEXTOUTPUT_H_ */