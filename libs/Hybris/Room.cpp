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
