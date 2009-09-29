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

#ifndef _MOBRIS_OBJECT_H_
#define _MOBRIS_OBJECT_H_

#include "types.h"
#include "Sprite.h"

struct GameInstance_t;

#define NUM_EVENTS 10

typedef enum {
	ET_LEFT			= 0,
	ET_RIGHT		= 1,
	ET_UP			= 2,
	ET_DOWN			= 3,
	ET_FIRE			= 4,
	ET_CREATE		= 5,
	ET_FRAME		= 6,
	ET_ANIMEND		= 7,
	ET_LEFTBORDER	= 8,
	ET_RIGHTBORDER	= 9
} EventType;

#define MAKE_EVENTMASK(a) ((1)<<(a))

typedef void (*Event)(struct GameInstance_t*);
typedef void (*CollisionEvent)(struct GameInstance_t*, struct GameInstance_t*);

struct GameObject_t;

typedef struct CollisionItem_t {
	CollisionEvent event;	
	struct GameObject_t* object;
} CollisionItem;

typedef struct GameObject_t {
	Sprite* sprite;
	Integer eventMask;
	Event events[NUM_EVENTS];
	CollisionItem* collisionItems;
	Integer numCollisionItems;
} GameObject;

GameObject Object_create() {
	GameObject ret;
	ret.sprite = 0;
	ret.eventMask = 0;
	return ret;
}

void Object_setEvent(GameObject* o, Integer type, Event e) {
	o->events[type] = e;
	o->eventMask |= MAKE_EVENTMASK(type);
}


#endif
