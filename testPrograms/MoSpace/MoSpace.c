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

void xyz() {
	MAMain();
}

#include "maapi.h"
#include "MAHeaders.h"
#include "Mobris.h"

// Player

Sprite playerSprite;
Handle playerSpriteHandle = xenon2;
GameObject playerObject;
GameInstance *playerInstance;

#define NUM_ENEMIES 50
Sprite enemy1Sprite;
Handle enemy1SpriteHandle = enemy1;
GameObject enemy1Object;
GameInstance *enemy1Instance[NUM_ENEMIES];

Sprite shot1Sprite;
Handle shot1SpriteHandle = shot;
GameObject shot1Object;
CollisionItem playerShotCollisionItems[1];

GameObject	explosionObject;
Sprite		explosionSprite;
Handle		explosionSpriteHandle = expl1;

GameRoom gameRoom;

Handle mapHandle;
int width, height, bound;

void up(GameInstance *instance)
{
	instance->y--;
	if(instance->y<0)
		instance->y = 0;
}

void down(GameInstance *instance)
{
	instance->y++;
	bound = height-SPRITE_HEIGHT(instance->object->sprite);
	if(instance->y>bound)
		instance->y = bound;
}

void left(GameInstance *instance)
{
	instance->x-=1;
	if(instance->x<0)
		instance->x = 0;
	else
	{
		instance->room->tileLayers[0].x--;
	}
}

void right(GameInstance *instance)
{

	instance->x+=1;
	bound = width-SPRITE_WIDTH(instance->object->sprite);
	if(instance->x>bound)
		instance->x = bound;
	else
	{
		instance->room->tileLayers[0].x++;
	}
}

static int shooting = 0;
void setShooting(GameInstance *inst)
{
	shooting = 0;
}

void fire(GameInstance *inst) {
	
	if(!shooting)
	{
		Room_createInstance(inst->room, &shot1Object, inst->x, inst->y, 0);
		scheduleTimerEvent(inst, setShooting, 25);
		shooting = 1;
	}
}

int dirs[NUM_ENEMIES];
int speeds[NUM_ENEMIES];
int curframe = 0;

void frame(GameInstance *instance) {
	int i = 0;

	gameRoom.tileLayers[0].y++;

	if(!(curframe&0xff))
	{
		for(i = 0; i < NUM_ENEMIES; i++)
		{
			speeds[i] = (((unsigned int)rand())%3);
			dirs[i] = -1+(((unsigned int)rand())%3);
		}

	}


	curframe++;
}

void enemyFrame(GameInstance *enemy)
{

	if(!(curframe&0x3))
	{
		enemy->y+=speeds[enemy->id];
		enemy->x+=dirs[enemy->id];
	}
}

void shotFrame(GameInstance *inst) {
	inst->y--;
	if(inst->y < 0) {
		Room_removeInstancePtr(inst->room, inst);
	}
}

void endExplosion(GameInstance *inst)
{
	printf("eeeeend\n");
	Room_removeInstancePtr(inst->room, inst);
}

void playerShotCollidedWithEnemy(GameInstance *playerShot, GameInstance *enemy)
{
	GameInstance *explosion;
	//explosion = Room_createInstance(enemy->room, &explosionObject, enemy->x, enemy->y, 0);
	Room_removeInstancePtr(enemy->room, enemy);
	Room_removeInstancePtr(enemy->room, playerShot);			
	//scheduleTimerEvent(explosion, endExplosion, 25);
}


void init()
{
	int i = 0;
	Extent scrSize = maGetScrSize();
	width = EXTENT_X(scrSize);
	height = EXTENT_Y(scrSize);

	playerSprite.images = &playerSpriteHandle;
	playerSprite.numImages = 1;
	playerObject = Object_create();
	playerObject.sprite = &playerSprite;

	Object_setEvent(&playerObject, ET_UP, up);
	Object_setEvent(&playerObject, ET_DOWN, down);
	Object_setEvent(&playerObject, ET_LEFT, left);
	Object_setEvent(&playerObject, ET_RIGHT, right);
	Object_setEvent(&playerObject, ET_FIRE, fire);
	Object_setEvent(&playerObject, ET_FRAME, frame);

	enemy1Sprite.images = &enemy1SpriteHandle;
	enemy1Sprite.numImages = 1;
	enemy1Object = Object_create();
	enemy1Object.sprite = &enemy1Sprite;
	Object_setEvent(&enemy1Object, ET_FRAME, enemyFrame);

	shot1Sprite.images = &shot1SpriteHandle;
	shot1Sprite.numImages = 1;
	shot1Object = Object_create();
	shot1Object.sprite = &shot1Sprite;
	Object_setEvent(&shot1Object, ET_FRAME, shotFrame);

	explosionSprite.images = &explosionSpriteHandle;
	explosionSprite.numImages = 1;
	explosionObject = Object_create();
	explosionObject.sprite = &explosionSprite;

	playerShotCollisionItems[0].event = playerShotCollidedWithEnemy;
	playerShotCollisionItems[0].object = &enemy1Object;
	shot1Object.numCollisionItems = 1;
	shot1Object.collisionItems = &playerShotCollisionItems;

	mapHandle = maInitLayer(map1set, map1, 10, 38);
	gameRoom = Room_create();
	Room_addTileLayer(&gameRoom, mapHandle, (width/2)-(10*24/2), -38*28+height, -0x50, 0x20);

	playerInstance = Room_createInstance(&gameRoom, &playerObject, (width/2)-(SPRITE_WIDTH((&playerSprite))/2), 10, 0);
	
	for(i = 0; i < NUM_ENEMIES; i++)
	{
		enemy1Instance[i] = Room_createInstance(&gameRoom, &enemy1Object, i*8, -(((unsigned int)rand())%(38*28)), i);
	}


	//maSoundInit(0, shotsound, 1);		

	//maSoundInit(1, music2, 1);	
	//maSoundInit(1, music1, 1);		

}

int MAMain()
{
	Extent scrSize = maGetScrSize();
	int width = EXTENT_X(scrSize); int w2 = width>>1; 
	int height = EXTENT_Y(scrSize); int h2 = height>>1;
	int i;
	int stime = maGetMilliSecondCount();
	int time;
	int frames = 0;
	int startedPlaying = 0;
	int triggered = 0;

	//maSoundStart(1);
	
	runGame(&gameRoom);

	return 0;
}
