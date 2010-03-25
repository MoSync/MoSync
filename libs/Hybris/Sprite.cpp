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

#include "Sprite.h"

namespace Hybris {
		Sprite::Sprite(int id, int sizeX, int sizeY, int numFrames, MAHandle image) : 
			id(id), 
			sizeX(sizeX),
			sizeY(sizeY),
			numFrames(numFrames),
			image(image) {
			}
			
		void Sprite::draw(int x, int y, int frame) {
			if(frame<0 || frame>=numFrames) return;
			
			MAExtent imageSize = maGetImageSize(image);
			int tx = (sizeX*frame)%(EXTENT_X(imageSize));
			int ty = ((sizeX*frame)/(EXTENT_X(imageSize)))*sizeY;
			MARect rect = {tx, ty, sizeX, sizeY};
			MAPoint2d dst = {x-(sizeX>>1), y-(sizeY>>1)};
			maDrawImageRegion(image, &rect, &dst, 0);
		}
		
		int Sprite::getId() {
			return id;
		}
		
		int Sprite::getWidth() {
			return sizeX;
		}
		
		int Sprite::getHeight() {
			return sizeY;
		}
		
}