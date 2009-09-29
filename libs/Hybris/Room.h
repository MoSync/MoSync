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
