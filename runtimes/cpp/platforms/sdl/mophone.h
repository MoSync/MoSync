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

#ifndef _MOPHONE_H_
#define _MOPHONE_H_

struct Skin;

struct Button {
	int button;
	int x, y, w, h;
};

#define NUM_BUTTONS 20
//extern Button buttons[NUM_BUTTONS];
//extern int realScreenStartX;
//extern int realScreenStartY;


bool isPointInsideScreen(int x, int y, int w, int h);
int containsButtonMophone(int x, int y);
int calculateScaleOfMophone(int w, int h);
void calculateMophone(int w, int h);
bool initMophoneScreen(Skin* skin, SDL_Surface **gScreen, int w, int h, bool shouldHaveMophone);
void drawButtonMophone(SDL_Surface *gScreen, int mak, bool pressed);
void freeMophone();
int getMophoneRealScreenStartX();
int getMophoneRealScreenStartY();
int getMophoneRealScreenEndX();
int getMophoneRealScreenEndY();
int getMophoneScale();

namespace Base {
	void pixelDoubledBlit(int x, int y, SDL_Surface *dstSurface, SDL_Surface *srcSurface, SDL_Rect srcRect, int multiplier);
}

#endif /* _MOPHONE_H_ */
