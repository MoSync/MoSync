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

#include "Room.h"

#include <conprint.h>

namespace Hybris {
	Room::Room(int id) : id(id) {
	}
		
	void Room::addCollisionListener(CollisionListener *cl) {
		collisionListeners.add(cl);
	}
		
	void Room::addSprite(Sprite *sprite) {
		sprites.add(sprite);
	}
		
	void Room::makeItem(int id, int spriteId, int x, int y) {
		int i = findSprite(spriteId);
		if(i==-1) return;
		Sprite *sprite = sprites[i];
		items.add(new Item(id, sprite, x, y));
	}
		
	void Room::killItem(int id) {
		int i = findItem(id);
		if(i==-1) return;
		Item *item = items[i];
		delete item;
		items.remove(i);
	}
		
	void Room::setItemPosition(int id, int x, int y) {
		int i = findItem(id);
		if(i==-1) return;
		Item *item = items[i];
		item->setPosition(x, y);
	}
	
	void Room::moveItemLeft(int id, int amt) {
		int i = findItem(id);
		if(i==-1) return;
		Item *item = items[i];
		item->moveLeft(amt);
	}
	
	void Room::moveItemUp(int id, int amt) {
		int i = findItem(id);
		if(i==-1) return;
		Item *item = items[i];
		item->moveUp(amt);	
	}
	
	void Room::moveItemRight(int id, int amt) {
		int i = findItem(id);
		if(i==-1) return;
		Item *item = items[i];
		item->moveRight(amt);		
	}
	
	void Room::moveItemDown(int id, int amt) {
		int i = findItem(id);
		if(i==-1) return;
		Item *item = items[i];
		item->moveDown(amt);		
	}
		
	void Room::setItemFrame(int id, int frame) {
		int i = findItem(id);
		if(i==-1) return;
		Item *item = items[i];
		item->setFrame(frame);		
	}
		
	void Room::update() {
		for(int i = 0; i < items.size(); i++) {
			for(int j = i+1; j < items.size(); j++) {
				if(items[i]->collideWith(items[j])) {
					fireOnItemCollision(i, items[i]->getSprite()->getId(), j, items[j]->getSprite()->getId());
				}
			}
		}
	}
		
	void Room::draw() {
		for(int i = 0; i < items.size(); i++) {
			items[i]->draw();
		}			
	}
		
	int Room::getId() {
		return id;
	}

	void Room::fireOnItemCollision(int itemId1, int spriteId1, int itemId2, int spriteId2) {
		for(int i = 0; i < collisionListeners.size(); i++) {
			collisionListeners[i]->onItemCollision(itemId1, spriteId1, itemId2, spriteId2);
		}
	}

	int Room::findItem(int id) {
		for(int i = 0; i < items.size(); i++) if(items[i]->getId() == id) { return i; }
		return -1;
	}
		
	int Room::findSprite(int id) {
		for(int i = 0; i < sprites.size(); i++) if(sprites[i]->getId() == id) { return i; }
		return -1;
	}

}
