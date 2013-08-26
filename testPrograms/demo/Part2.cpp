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