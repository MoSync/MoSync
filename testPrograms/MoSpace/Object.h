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
