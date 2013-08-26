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