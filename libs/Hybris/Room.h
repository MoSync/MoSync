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

#ifndef _HYBRIS_ROOM_
#define _HYBRIS_ROOM_

#include "Sprite.h"
#include "Item.h"
#include <MAUtil/Vector.h>

namespace Hybris {	
	using namespace MAUtil;
	
	class CollisionListener {
		public:
			virtual void onItemCollision(int itemId1, int spriteId1, int itemId2, int spriteId2) = 0;
	};

	class Room {
	public:
		Room(int id);
		void addCollisionListener(CollisionListener *cl);
		void addSprite(Sprite *sprite);
		void makeItem(int id, int spriteId, int x, int y);
		void killItem(int id);
		void setItemPosition(int id, int x, int y);
		void moveItemLeft(int id, int amt);
		void moveItemUp(int id, int amt);
		void moveItemRight(int id, int amt);		
		void moveItemDown(int id, int amt);		
		void setItemFrame(int id, int frame);
		void update();
		void draw();
		int getId();
		
	private:
		void fireOnItemCollision(int itemId1, int spriteId1, int itemId2, int spriteId2);
		int findItem(int id);
		int findSprite(int id);
		
		int id;
		Vector<Item*> items;
		Vector<CollisionListener*> collisionListeners;
		Vector<Sprite*> sprites;
	};
}

#endif
