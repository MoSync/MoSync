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

#include "Part2.h"
#include <mastdlib.h>
#include <conprint.h>
#include "MAHeaders.h"

struct PlanetBucket {
	int size;
	Planet *planets[NUM_PLANETS];
};

PlanetBucket buckets[NUM_PLANET_BUCKETS];
Planet planets[NUM_PLANETS];
int screenWidth, screenHeight;

Part2::Part2() {
	Extent s = maGetScrSize(); 
	screenWidth = EXTENT_X(s); 
	screenHeight = EXTENT_Y(s);	
	
	double angDelta = (2*3.14159)/(double)NUM_PLANETS;
	double ang = 0.0;

	for(int i = 0; i < NUM_PLANETS; i++) {
		planets[i].type = rand()&0x3;
		planets[i].x = cos(ang)*200;//(rand()%1500)-(1500>>1);
		planets[i].y = sin(ang)*200;//(rand()%1500)-(1500>>1);
		planets[i].startz = i*32; //rand()&0x3ff;
		//planets[i].startz = 0;
		ang+=angDelta;
	}
	
	int h = R_PLANET1_START;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			planetImages[i][j] = h+j;
		}
		h+=4;
	}
}

void Part2::render(int time, double dtime) {
	for(int i = 0; i < NUM_PLANET_BUCKETS; i++) buckets[i].size = 0;
	for(int i = 0; i < NUM_PLANETS; i++) {
		planets[i].z = planets[i].startz + (time>>1);
		planets[i].z&=0x3ff;
		int bucket = (planets[i].z>>4);
		planets[i].z = 1023-planets[i].z;
		buckets[bucket].planets[buckets[bucket].size++] = &planets[i];
	}
	
	//maSetColor(0);
	//maFillRect(0, 0, screenWidth, screenHeight);
	Extent s = maGetImageSize(R_PART2_BKG);
	int x = ((screenWidth>>1)-(EXTENT_X(s)>>1)) + (int)(cos(dtime*3)*8);
	int y = ((screenHeight>>1)-(EXTENT_Y(s)>>1)) + (int)(sin(dtime*2)*8);
	maDrawImage(R_PART2_BKG, x, y);

	for(int i = 0; i < NUM_PLANET_BUCKETS; i++) {
		for(int j = 0; j < buckets[i].size; j++) {
			int x = buckets[i].planets[j]->x;
			int y = buckets[i].planets[j]->y;
			int z = buckets[i].planets[j]->z;
			
			if(z!=0) {
/*			
				int rz = ((0 xff)<<8)/(z);
				int sx = ((x*rz)>>8) + (screenWidth>>1);
				int sy = ((y*rz)>>8) + (screenHeight>>1);
*/	
				int sx = ((((x)<<10)/(z))>>4) + (screenWidth>>1);
				int sy = ((((y)<<10)/(z))>>4) + (screenHeight>>1);

				//int zoom = 3;
				//int zo = 16;
				int zoom = 3-(((4*0xffffff)/(z<<1))>>16);
				if(zoom<0) zoom = 0; if(zoom>3) zoom = 3;
				int zo = 64-(zoom<<4);

				maDrawImage(planetImages[buckets[i].planets[j]->type][zoom], sx-(zo>>1), sy-(zo>>1));
			}
		}
	}
	
	
}