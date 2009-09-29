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

#ifndef TILE_H
#define TILE_H

#include <helpers/array.h>
#include "TileImpl.h"

class TileSet
{
public:
	TileSet(TILE_SET *tileSet, int tileWidth, int tileHeight);
	~TileSet();

	TILE_SET *tileSet;
	int tileWidth;
	int tileHeight;
};

class TileMap
{
public:
	TileMap(const void*, int tileMapWidth, int tileMapHeight);
	~TileMap();

	unsigned short *tileMap;
	int tileMapWidth;
	int tileMapHeight;
	int tileMapSize;
};

struct TileLayer
{
	TileMap *tileMap;
	TileSet *tileSet;
	bool active;
};

#endif // TILE_H
