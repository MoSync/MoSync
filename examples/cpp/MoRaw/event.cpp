#include <ma.h>
#include "event.h"

bool EventHandler::left_pressed =false;
bool EventHandler::right_pressed =false;
bool EventHandler::up_pressed =false;
bool EventHandler::down_pressed =false;
bool EventHandler::fire_pressed =false;
bool EventHandler::pound_pressed =false;
bool EventHandler::star_pressed =false;

bool EventHandler::left =false;
bool EventHandler::right =false;
bool EventHandler::up =false;
bool EventHandler::down =false;
bool EventHandler::fire =false;
bool EventHandler::quit =false;
bool EventHandler::updated =false;
bool EventHandler::pound =false;
bool EventHandler::star =false;
bool EventHandler::sleft =false;
bool EventHandler::sright =false;

void EventHandler::updateEvents() {
	left_pressed = right_pressed = up_pressed = down_pressed = fire_pressed = pound_pressed = star_pressed = false;
	updated = false;
	MAEvent event;
	/// update key states
	while((maGetEvent(&event))!=0) {
		updated = true;
		if(event.type == EVENT_TYPE_CLOSE) {
			quit = true;
			break;
		}

		if(event.type == EVENT_TYPE_KEY_PRESSED) switch(event.key) {
		case MAK_5:
		case MAK_FIRE:
			fire = true;
			fire_pressed = true;
			break;
		case MAK_UP:
		case MAK_2:
			left = true;
			left_pressed = true;
			break;
		case MAK_DOWN:
		case MAK_8:
			right = true;
			right_pressed = true;
			break;
		case MAK_RIGHT:
		case MAK_6:
			up = true;
			up_pressed = true;
			break;
		case MAK_LEFT:
		case MAK_4:				
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
			sleft = true;
			break;
		case MAK_SOFTRIGHT:
			sright = true;
			break;
		}
		if(event.type == EVENT_TYPE_KEY_RELEASED) switch(event.key) {
		case MAK_5:
		case MAK_FIRE:
			fire = false;
			break;
		case MAK_UP:
		case MAK_2:
			left = false;
			break;
		case MAK_DOWN:
		case MAK_8:
			right = false;
			break;
		case MAK_RIGHT:
		case MAK_6:
			up = false;
			break;
		case MAK_LEFT:
		case MAK_4:
			down = false;
			break;
		case MAK_POUND:
			pound = false;
			break;
		case MAK_STAR:
			star = false;
			break;
		case MAK_SOFTLEFT:
			sleft = false;
			break;
		case MAK_SOFTRIGHT:
			sright = false;
			break;
		}
	}
}
