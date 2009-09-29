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

class Syscall {
private:
#ifdef MOBILEAUTHOR
public:
#endif
SDL_Surface* loadImage(MemStream& s);
SDL_Surface* loadSprite(SDL_Surface* surface, ushort left, ushort top,
	ushort width, ushort height, ushort cx, ushort cy);
TileSet* loadTileSet(MemStream& s, ushort tileWidth, ushort tileHeight);
TileMap* loadTileMap(MemStream& s, ushort width, ushort height);

public:
	struct STARTUP_SETTINGS {
		STARTUP_SETTINGS() : showScreen(true), shouldHaveMophone(true), id(NULL),
			iconPath(NULL), resmem((uint)-1)
		{}

		bool showScreen, shouldHaveMophone;
		const char* id;
		const char *iconPath;
		uint resmem;
		const char *vendor;
		const char *model;
	};

	Syscall(const STARTUP_SETTINGS&);
	Syscall(int width, int height, const STARTUP_SETTINGS&);
