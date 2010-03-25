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