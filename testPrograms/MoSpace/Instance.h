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

#ifndef _MOBRIS_INSTANCE_H_
#define _MOBRIS_INSTANCE_H_

#include "types.h"
#include "Object.h"

struct GameRoom_t;

#define MAXVARS 16

/* Sizeof(instance):
*/
//TODO: add instance variables

Integer g_currFrame = 0;


typedef struct GameInstance_t {
	GameObject *object;
	Integer x, y;
	Integer startFrame;
	Integer id;
	struct GameRoom_t *room;
} GameInstance;


void doEvent(GameInstance* inst, Integer type) {
	if(inst->object->eventMask & MAKE_EVENTMASK(type)) {
		//executeAction(inst->object->events[type]->action, inst);
		Event e = inst->object->events[type];
		e(inst);
	}
}

typedef struct TimerEvent_t {
	GameInstance* inst;
	Event* e;
	Integer time; // in milliseconds
} TimerEvent;

#define MAX_TIMER_EVENTS 16

Integer numTimerEvents = 0;

TimerEvent timerEventPool[MAX_TIMER_EVENTS];

void clearTimerEventPool() {
	numTimerEvents = 0;
}

void scheduleTimerEvent(GameInstance* inst, Event* e, Integer deltaTime) {
	if(numTimerEvents < MAX_TIMER_EVENTS) {
		timerEventPool[numTimerEvents].inst = inst;
		timerEventPool[numTimerEvents].e = e;
		timerEventPool[numTimerEvents].time = maGetMilliSecondCount()+deltaTime*20;
		numTimerEvents++;
	}
}

void removeTimerEvent(Integer index) {
	timerEventPool[index] = timerEventPool[--numTimerEvents];
}

void runTimerEvents() {
	Integer currTime = maGetMilliSecondCount();
	Integer i;
	for(i = 0; i < numTimerEvents; i++) {
		TimerEvent* e = &timerEventPool[i];
		if(currTime >= e->time) {
			Event func = e->e;
			func(e->inst);
			e++;
			removeTimerEvent(i);
		}
	}
}

void renderInstance(GameInstance* inst, Integer frame) {
	Integer myFrame = frame - inst->startFrame;

	Sprite *sprite = inst->object->sprite;

	Integer animFrame = myFrame*sprite->animSpeed;
	Handle h = sprite->images[(animFrame)%sprite->numImages];

	
	
	if(sprite->numImages == 6) {
		//////printf("g_currFrame: %d\n", g_currFrame);		
		//////printf("animFrame: %d\n", animFrame);
		//////printf("myFrame: %d\n", myFrame);
		//////printf("animSpeed: %d\n", sprite->animSpeed);
	}

	if(animFrame > sprite->numImages) {
		doEvent(inst, ET_ANIMEND);
	}

	maDrawImage(h, inst->x, inst->y); 
}

Bool Instance_collides(GameInstance* i1, GameInstance *i2) {
	Sprite* s1 = i1->object->sprite;
	Sprite* s2 = i2->object->sprite;
	Integer l1 = i1->x;
	Integer l2 = i2->x;
	Integer t1 = i1->y;
	Integer t2 = i2->y;
	Integer r1 = l1+SPRITE_WIDTH(s1);
	Integer r2 = l2+SPRITE_WIDTH(s2);
	Integer b1 = t1+SPRITE_HEIGHT(s1);
	Integer b2 = t2+SPRITE_HEIGHT(s2);	

    if (b1 < t2) return 0;
    if (t1 > b2) return 0;

    if (r1 < l2) return 0;
    if (l1 > r2) return 0;

	return 1;
}

void Instance_doCollision(GameInstance* i1, GameInstance* i2) {
	GameObject *o1 = i1->object;
	GameObject *o2 = i2->object;
	Integer i;
	// see if i1 has a handler for collisions with i2
	if(o1 == o2) {
		for(i = 0; i < o1->numCollisionItems; i++)
			if(o1->collisionItems[i].object == o2) {
				o1->collisionItems[i].event(i1, i2);
				o1->collisionItems[i].event(i2, i1);
			}
		return;
	}
	for(i = 0; i < o1->numCollisionItems; i++)
		if(o1->collisionItems[i].object == o2) {
			////printf("calling a collision event 1  !!\n");
			o1->collisionItems[i].event(i1, i2);
		}
	for(i = 0; i < o2->numCollisionItems; i++)
		if(o2->collisionItems[i].object == o1) {
			////printf("calling a collision event 2  !!\n");
			o2->collisionItems[i].event(i2, i1);
		}
}


#endif
