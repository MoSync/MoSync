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

#include "Game.h"

namespace Hybris {
	Game::Game() : currentRoom(0) {
	}
			
	void Game::addRoom(Room *room) {
		rooms.add(room);
		currentRoom = room;
	}
			
	void Game::setCurrentRoom(int id) {
		for(int i = 0; i < rooms.size(); i++) {
			if(id == rooms[i]->getId()) { 
				currentRoom = rooms[i];
				break;
			}
		}
	}
			
	void Game::update() {
		if(currentRoom)
			currentRoom->update();
	}
			
	void Game::draw() {
		if(currentRoom)
			currentRoom->draw();			
	}
		
}