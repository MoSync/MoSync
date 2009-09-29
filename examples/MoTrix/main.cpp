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
* Shows how to use draw image region in a rather adanved way.
*
* Copyright (c) Mobile Sorcery AB 2005-2007
*
* \author Niklas Nummelin
*
*/
#include <ma.h>
#include <mastdlib.h>
#include "MAHeaders.h"

/// change this to get different frequency of drops
#define MAX_DROPS_TO_APPEAR 4
#define MAX_TIME_TO_NEXT_DROP 1
#define UPDATES_PER_SECOND 10

/// defined by the characters set
#define NUM_LETTERS (16*2)
#define NUM_INTENSITIES 8

/**
 *	The size of the characters map in the x-direction.
 */
int tileMapSizeX;

/**
 *	The size of the characters map in the y-direction.
 */
int tileMapSizeY;

/**
 * Struct holding the information about each matrix-type character.
 * Used to calculate the right index, and recalculate intensities.
 */
typedef struct {
	int intensity, character;
}MatrixChar;

MAHandle tileLayer;

MARect matrixCharToTileMapLut[NUM_LETTERS][NUM_INTENSITIES];

/**
 * The array holding the matrix characters.
 */
MatrixChar *characters;

int nextDrop = 0;

/**
 * Time until the next set of drops will appear.
 */

extern "C" {
	/**
	* Init the matrix character characters array.
	*/
	void initChars() {
		MAExtent e = maGetScrSize();
		tileMapSizeX = EXTENT_X(e)/16;
		tileMapSizeY = EXTENT_Y(e)/16;

		characters = new MatrixChar[tileMapSizeX*tileMapSizeY];
		memset(characters, 0, sizeof(MatrixChar)*tileMapSizeX*tileMapSizeY);

		for(int i = 0; i < NUM_INTENSITIES; i++) {
			for(int j = 0; j < NUM_LETTERS; j++) {
			matrixCharToTileMapLut[j][i].left = (j&0xf)<<4; 
			matrixCharToTileMapLut[j][i].top = (i<<5) + (j>>1); //((i<<1) + (j>>4))<<4;
			matrixCharToTileMapLut[j][i].width = 16; 
			matrixCharToTileMapLut[j][i].height = 16;

			}
		}

		nextDrop = maGetMilliSecondCount();
	}

	void deleteChars() {
		delete characters;
	}

	/**
	* Recalculate intensities and update characters.
	*/
	void drawChars() {

		int time = maGetMilliSecondCount();

		if(time >= nextDrop) {

			nextDrop = time + (rand()%MAX_TIME_TO_NEXT_DROP);
			int numDropsToAppear = rand()%MAX_DROPS_TO_APPEAR;
			for(int j = 0; j < numDropsToAppear; j++) {
				int x = rand()%tileMapSizeX;
				characters[x].intensity = NUM_INTENSITIES-1;
				characters[x].character = rand()&(NUM_LETTERS-1);
			}
		}
	
		int index = -1;
		for(int j = 0; j < tileMapSizeY-1; j++) {
			for(int i = 0; i < tileMapSizeX; i++) {
				index++;
				int intensity = characters[index].intensity;
				if(!intensity) continue;
				intensity--;
				if(intensity>0) {			
					characters[index].intensity = intensity;
					MAPoint2d dstPoint = {i<<4, j<<4};//MAPoint2d dstPoint = {i*16, j*16};	
					maDrawImageRegion(R_MOTRIX, &matrixCharToTileMapLut[characters[index].character][characters[index].intensity], &dstPoint, 0); 
				} else {	
					characters[index].intensity = 0;
				}

				intensity+=2;
				if(intensity>(NUM_INTENSITIES-1)) continue;
				characters[index + tileMapSizeX].intensity = intensity;
				characters[index + tileMapSizeX].character = rand()&(NUM_LETTERS-1);
			}
		}

		/// do the last line
		for(int i = 0; i < tileMapSizeX; i++) {
			index++;
			int intensity = characters[index].intensity;
			if(!intensity) continue;
			intensity--;
			if(intensity>0) {			
				characters[index].intensity = intensity;
				MAPoint2d dstPoint = {i<<4, (tileMapSizeY-1)<<4};//MAPoint2d dstPoint = {i*16, j*16};	
				maDrawImageRegion(R_MOTRIX, &matrixCharToTileMapLut[characters[index].character][characters[index].intensity], &dstPoint, 0); 
			} else {	
				characters[index].intensity = 0;
			}
		}
	}

/**
 * \brief The entry point.
 */
int MAMain()
{
	MAExtent e = maGetScrSize();

	initChars();

	while(1) {
		MAEvent event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				deleteChars();
				return 0;
			}
		}

		maSetColor(0);
		maFillRect(0, 0, EXTENT_X(e), EXTENT_Y(e));

		drawChars();

		/// Updates the screen
		maUpdateScreen();

		/// Keep the backlight alive.
		maResetBacklight();

#ifndef MAPIP
		maWait(40);
#endif
	}


	return 0;
}
}
