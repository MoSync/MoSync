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

/** \file main.cpp
*
* This file contains the main example program source.
*
* Shows how to use tile sets and tile maps.
*
* Copyright (c) Mobile Sorcery AB 2005-2007
*
* \author Niklas Nummelin
*
*/
#include <ma.h>
#include <mastdlib.h>
#include "MAHeaders.h"

/**
 * \brief The entry point.
 */
extern "C" int MAMain()
{
	MAExtent e = maGetScrSize();
	int width = EXTENT_X(e);
	int height = EXTENT_Y(e);

	const int tileMapSizeX = 24;
	const int tileMapSizeY = 64;
	const int tileSetSizeX = 16;
	const int tileSetSizeY = 16;
	const int numTiles = 64;

	/// create the tilemap layer, the map is the R_TILEMAP resource
	MAHandle foreGroundLayer  = maInitLayer(R_TILESET, R_TILEMAP, tileMapSizeX, tileMapSizeY);
	MAHandle backGroundLayer  = maInitLayer(R_TILESET, R_TILEMAP, tileMapSizeX, tileMapSizeY);

	/// fill tile map with random tiles.
	
	for(int i = 0; i < tileMapSizeX; i++) {
		for(int j = 0; j < tileMapSizeY; j++) {
			// zero means no tile
			// we want all to be filled in the background
			// and some in the foreground
			maSetTile(backGroundLayer, i, j, 1 + (rand()%numTiles));

			// if some random value is over 10 we want no tile 
			// there (to get less visible tiles in the foreground layer)
			int r = (rand()%(numTiles+1));
			if(r>10) r = 0;
			maSetTile(foreGroundLayer, i, j, r);
		}
	}

	int x = 0, bX = 0; 
	int y = 0, bY = 0;
	int factor = 2;

	bool left = false;
	bool right = false; 
	bool up = false;
	bool down = false;

	while(1) {
		MAEvent event;
		/// update key states
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_KEY_PRESSED) {
				switch(event.key) {
				case MAK_LEFT:
				case MAK_4:
					left = true;
					break;
				case MAK_RIGHT:
				case MAK_6:
					right = true;
					break;
				case MAK_UP:
				case MAK_2:
					up = true;
					break;
				case MAK_DOWN:
				case MAK_8:				
					down = true;
					break;
				}
			} else if(event.type == EVENT_TYPE_KEY_RELEASED) {
				switch(event.key) {
				case MAK_LEFT:
				case MAK_4:
					left = false;
					break;
				case MAK_RIGHT:
				case MAK_6:
					right = false;
					break;
				case MAK_UP:
				case MAK_2:
					up = false;
					break;
				case MAK_DOWN:
				case MAK_8:
					down = false;
					break;
				case MAK_0:
					return 0;
				}
			} else if(event.type == EVENT_TYPE_CLOSE) {
				return 0;
			}
		}
		
		/// update offsets
		if(left) {
			if(x<0) {x+=factor; bX+=factor>>1; }
			if(x>0) {x=0; bX = 0;}
		}
		if(right) {
			if(x>-tileMapSizeX*tileSetSizeX+width) {x-=factor; bX-=factor>>1; }
			if(x<-tileMapSizeX*tileSetSizeX+width) {x = -tileMapSizeX*tileSetSizeX+width; bX=x; }
		}
		if(up) {
			if(y<0) {y+=factor; bY+=factor>>1; }
			if(y>0) {y=0; bY = 0;}
		}
		if(down) {
			if(y>-tileMapSizeY*tileSetSizeY+height) {y-=factor; bY-=factor>>1; }
			if(y<-tileMapSizeY*tileSetSizeY+height) {y=-tileMapSizeY*tileSetSizeY+height; bY = y; }
		}

		/// draw layers and pass the current offsets
		maDrawLayer(backGroundLayer, bX, bY);
		maDrawLayer(foreGroundLayer, x, y);

		/// Updates the screen
		maUpdateScreen();

		/// Keep the backlight alive.
		maResetBacklight();
	
#ifndef MAPIP
		maWait(1000/50);
#endif
	}
	return 0;
}
