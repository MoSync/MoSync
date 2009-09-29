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

#ifndef _HYBRIS_GAME_
#define _HYBRIS_GAME_

#include "Room.h"
#include <MAUtil/Vector.h>

namespace Hybris {
	using namespace MAUtil;

	class Game {
		public:
			Game();
			void addRoom(Room *room);
			void setCurrentRoom(int id);
			void update();
			void draw();
			
		private:
			Room *currentRoom;
			Vector<Room*> rooms;
	};	
}

#endif
