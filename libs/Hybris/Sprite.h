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

#ifndef _HYBRIS_SPRITE_
#define _HYBRIS_SPRITE_

#include <ma.h>

namespace Hybris {
	class Sprite {
	public:
		Sprite(int id, int sizeX, int sizeY, int numFrames, MAHandle image);
		void draw(int x, int y, int frame);
		int getId();
		int getWidth();
		int getHeight();
	private:
		MAHandle image;
		int id;
		int sizeX;
		int sizeY;
		int numFrames;
			
	};
}

#endif
