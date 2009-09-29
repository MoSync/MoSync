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

#ifndef _MOBRIS_ROOM_H_
#define _MOBRIS_ROOM_H_

#include "Instance.h"

#define MAX_INSTANCES 128
#define MAX_TILE_LAYERS 8

typedef struct TileLayer_t {
	Handle layer;
	int xscale;
	int yscale;
	int startx;
	int starty;
	int x;
	int y;
} TileLayer;

/* GameInstance instancePool[MAX_INSTANCES]; */
/* Integer instancePoolSize;  */

typedef struct GameRoom_t {	
	Handle background;
	Bool tile_x;
	Bool tile_y;
	Integer width;
	Integer height;
	GameInstance instancePool[MAX_INSTANCES];
	Integer instancePoolSize;

	TileLayer tileLayers[MAX_TILE_LAYERS];
	int numLayers;
} GameRoom;

GameRoom Room_create() {
	GameRoom ret;
	ret.background = 0;
	ret.tile_x = 0;
	ret.tile_y = 0;
	ret.width = 170;
	ret.height = 200;
	ret.instancePoolSize = 0;
	ret.numLayers = 0;
	return ret;
}

void Room_addTileLayer(GameRoom *r, Handle layer, int x, int y, int xscale, int yscale)
{
	r->tileLayers[r->numLayers].layer = layer;
	r->tileLayers[r->numLayers].startx = x;
	r->tileLayers[r->numLayers].starty = y;
	r->tileLayers[r->numLayers].x = 0;
	r->tileLayers[r->numLayers].y = 0;
	r->tileLayers[r->numLayers].xscale = xscale;
	r->tileLayers[r->numLayers].yscale = yscale;
	r->numLayers++;
}

void Room_clearPool(GameRoom *r) {
	r->instancePoolSize = 0;
}

void Room_insertInstance(GameRoom* r, GameInstance* inst) {
	if(r->instancePoolSize < MAX_INSTANCES) {
		r->instancePool[r->instancePoolSize] = *inst;
		r->instancePool[r->instancePoolSize].room = r;
		r->instancePool[r->instancePoolSize].startFrame = g_currFrame;
		r->instancePoolSize++;
		doEvent(&r->instancePool[r->instancePoolSize-1], ET_CREATE);
	}
}

GameInstance* Room_createInstance(GameRoom* r, GameObject *obj, Integer x, Integer y, Integer id) {
	if(r->instancePoolSize < MAX_INSTANCES) {
		r->instancePool[r->instancePoolSize].object = obj;
		r->instancePool[r->instancePoolSize].x = x;
		r->instancePool[r->instancePoolSize].y = y;
		r->instancePool[r->instancePoolSize].id = id;			
		r->instancePool[r->instancePoolSize].room = r;
		r->instancePool[r->instancePoolSize].startFrame = g_currFrame;
		r->instancePoolSize++;
		doEvent(&r->instancePool[r->instancePoolSize-1], ET_CREATE);
		return &r->instancePool[r->instancePoolSize-1];
	}
	return 0;	
}

void Room_removeInstance(GameRoom *r, Integer index) {
	r->instancePool[index] = r->instancePool[--r->instancePoolSize];
	//printf("instancePool now has size %d\n", r->instancePoolSize);
}

void Room_removeInstancePtr(GameRoom *r, GameInstance* inst) {
	Integer index = ((Integer)inst-(Integer)(r->instancePool))/sizeof(GameInstance);
	Room_removeInstance(r, index);
}


void Room_doCollisions(GameRoom* r) {

	//TODO: Fix the collision stuff :/
	Integer i,j;
	GameInstance* instances = r->instancePool;

	for(i = 0; i < r->instancePoolSize-1; i++) {
		for(j = i+1; j < r->instancePoolSize; j++) {
			if(Instance_collides(&instances[i], &instances[j])) {
				Instance_doCollision(&instances[i], &instances[j]);
				//coll = 1;
				//break;
			}
		}
		//if(coll) break;
	}

	// collisions with borders
	for(i = 0; i < r->instancePoolSize; i++) {
		if(instances[i].x <= 0) {
			doEvent(&instances[i], ET_LEFTBORDER);
		}
		if(instances[i].x+SPRITE_WIDTH(instances[i].object->sprite) >= EXTENT_X(maGetScrSize())) {
			doEvent(&instances[i], ET_RIGHTBORDER);
		}
	}
}

void Room_render(GameRoom* r, Integer frame) {
	GameInstance* pool = r->instancePool;
	Integer n = r->instancePoolSize;
	Integer i;

	runTimerEvents();
	Room_doCollisions(r);

	for(i = 0; i < r->numLayers; i++)
	{
		TileLayer *layer = &r->tileLayers[i];
		maDrawLayer(layer->layer,layer->startx+((layer->x*layer->xscale)>>8), layer->starty+((layer->y*layer->yscale)>>8));
	}

	while(n--) {
		renderInstance(&pool[n], frame);
	}
}

void Room_event(GameRoom* r, Integer eventType) {
	GameInstance* pool = r->instancePool;
	Integer n = r->instancePoolSize;

	while(n--) {
		doEvent(&pool[n], eventType);
	}
}

#endif
