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

#include <ma.h>

#include "event.h"

bool EventHandler::left_pressed =false;
bool EventHandler::right_pressed =false;
bool EventHandler::up_pressed =false;
bool EventHandler::down_pressed =false;
bool EventHandler::fire_pressed =false;
bool EventHandler::pound_pressed =false;
bool EventHandler::star_pressed =false;
bool EventHandler::lsk_pressed=false;
bool EventHandler::rsk_pressed=false;

bool EventHandler::left =false;
bool EventHandler::right =false;
bool EventHandler::up =false;
bool EventHandler::down =false;
bool EventHandler::fire =false;
bool EventHandler::quit =false;
bool EventHandler::updated =false;
bool EventHandler::pound =false;
bool EventHandler::star =false;
bool EventHandler::lsk=false;
bool EventHandler::rsk=false;

bool EventHandler::focus = true;

#ifdef MA_PROF_SUPPORT_STYLUS
bool EventHandler::pointer_pressed=false;
bool EventHandler::pointer_released=false;
MAPoint2d EventHandler::point;
#endif	// MA_PROF_SUPPORT_STYLUS

void EventHandler::updateEvents() {
	left_pressed = right_pressed = up_pressed = down_pressed = fire_pressed = pound_pressed = star_pressed = lsk_pressed = rsk_pressed = false;
	updated = false;
	MAEvent event;
	/// update key states
	while(maGetEvent(&event)) {
		updated = true;
		if(event.type == EVENT_TYPE_CLOSE)
			quit = true;
		if(event.type == EVENT_TYPE_FOCUS_LOST)
			focus = false;
		if(event.type == EVENT_TYPE_FOCUS_GAINED)
			focus = true;

		if(event.type == EVENT_TYPE_KEY_PRESSED) switch(event.key) {
		case MAK_5:
		case MAK_FIRE:
			fire = true;
			fire_pressed = true;
			break;
		case MAK_LEFT:
		case MAK_4:
			left = true;
			left_pressed = true;
			break;
		case MAK_RIGHT:
		case MAK_6:
			right = true;
			right_pressed = true;
			break;
		case MAK_UP:
		case MAK_2:
			up = true;
			up_pressed = true;
			break;
		case MAK_DOWN:
		case MAK_8:				
			down = true;
			down_pressed = true;
			break;
		case MAK_POUND:
			pound = true;
			pound_pressed = true;
			break;
		case MAK_STAR:
			star = true;
			star_pressed = true;
			break;
		case MAK_SOFTLEFT:
			lsk = true;
			lsk_pressed = true;
			break;
		case MAK_SOFTRIGHT:
			rsk = true;
			rsk_pressed = true;
			break;
		}
		if(event.type == EVENT_TYPE_KEY_RELEASED) switch(event.key) {
		case MAK_5:
		case MAK_FIRE:
			fire = false;
			break;
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
		case MAK_POUND:
			pound = false;
			break;
		case MAK_STAR:
			star = false;
			break;
		case MAK_SOFTLEFT:
			lsk = false;
			break;
		case MAK_SOFTRIGHT:
			rsk = false;
			break;
		}
#ifdef MA_PROF_SUPPORT_STYLUS
		if(event.type == EVENT_TYPE_POINTER_PRESSED) {
			pointer_pressed = true;
			point.x = event.point.x;
			point.y = event.point.y;
		}
		if(event.type == EVENT_TYPE_POINTER_RELEASED) {
			EventHandler::left = EventHandler::up = EventHandler::right = EventHandler::down = false;
			pointer_pressed = false;
			point.x = event.point.x;
			point.y = event.point.y;
		}
#endif	// MA_PROF_SUPPORT_STYLUS
	}
}
