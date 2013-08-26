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

#ifndef _HYBRIS_ITEM_
#define _HYBRIS_ITEM_

#include "Sprite.h"
#include <MAUtil/Geometry.h>

namespace Hybris {

	class Item {
		public:
			Item(int id, Sprite *sprite, int x, int y);
			int getId();
			Sprite* getSprite();
			bool collideWith(Item *i);
			void draw();
			const MAUtil::Point& getPosition() const;		
			void setPosition(int x, int y);	
			void moveUp(int amt);
			void moveLeft(int amt);
			void moveRight(int amt);
			void moveDown(int amt);
			
			void setFrame(int frame);
			void setAnimation(int startFrame, int startOfAnim, int numFrames);
			void stepAnimationForward();
			void stepAnimationBackwards();
		private:
			int id, frame;
			MAUtil::Point p;
			Sprite *sprite;
			
			int startOfAnim, numFrames;
	};
		
}

#endif
		