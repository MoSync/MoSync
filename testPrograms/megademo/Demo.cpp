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

#include "Demo.h"
#include <MAUtil/Vector.h>
using namespace MAUtil;

static Vector<Effect*> sEffects;
static int sEffectIndex = 0;

void Surface::blit(Surface *surf, int x, int y, int flags) {
	int imageWidth = surf->w;
	int imageHeight = surf->h;
	int surfaceWidth = this->w;
	int surfaceHeight = this->h;
	byte* img = surf->pixels;
	byte* backBuffer = pixels;

	int topLeftX = x-(imageWidth>>1);
	int topLeftY = y-(imageHeight>>1);
	int bottomRightX = x+(imageWidth>>1);
	int bottomRightY = y+(imageHeight>>1);
	int realWidth = imageWidth;
	int realHeight = imageHeight;

	if(topLeftX>surfaceWidth) return;
	if(topLeftY>surfaceHeight) return;
	if(bottomRightX<0) return;
	if(bottomRightY<0) return;

	int offsetX = 0;
	int offsetY = 0;

	if(topLeftX<0) {
		offsetX = -topLeftX;
		topLeftX = 0;
		realWidth -= offsetX;
	}

	if(topLeftY<0) {
		offsetY = -topLeftY;
		topLeftY = 0;
		realHeight -= offsetY;
	}

	if(offsetX>=imageWidth)
		return;
	if(offsetY>=imageHeight)
		return;

	if(bottomRightX>=surfaceWidth)
		realWidth -= (bottomRightX - surfaceWidth);

	if(bottomRightY>=surfaceHeight)
		realHeight -= (bottomRightY - surfaceHeight);

	if(realWidth<=0) return;
	if(realHeight<=0) return;

	img += offsetX + offsetY * imageWidth;
	backBuffer += topLeftX + topLeftY * surfaceWidth;

	switch(flags&0x1) {
	case BLIT_REPLACE:
		for(int j = 0; j < realHeight; j++) {
			for(int i = 0; i < realWidth; i++) {
				backBuffer[i] = img[i];
			}
			img += imageWidth;
			backBuffer += surfaceWidth;
		}
		break;
	case BLIT_ADD:
		for(int j = 0; j < realHeight; j++) {
			for(int i = 0; i < realWidth; i++) {
				int c = (int) backBuffer[i] + (int) img[i];
				if(c>255) c = 255;
				backBuffer[i] = (byte)c;
			}
			img += imageWidth;
			backBuffer += surfaceWidth;
		}
		break;
	}
}


float startTime = 0.0;

void addEffect(Effect* effect) {
	sEffects.add(effect);
}

void setEffectIndex(int i) {
	if(i<0) i = 0;
	if(i>=sEffects.size()) i = sEffects.size()-1;
	sEffectIndex = i;
	if(!sEffects.size()) return;
	startTime = maGetMilliSecondCount()*0.001;
	sEffects[sEffectIndex]->prepare();
}

int getEffectIndex() {
	return sEffectIndex;
}

void initEffects(Surface* surf) {
	for(int i = 0; i < sEffects.size(); i++) {
		sEffects[i]->init(surf);
	}
	setEffectIndex(0);
}

void renderEffect(Surface* surface) {
	if(sEffects.size()==0) return;
	sEffects[sEffectIndex]->render(surface, (float)maGetMilliSecondCount()*0.001f - startTime);
}
