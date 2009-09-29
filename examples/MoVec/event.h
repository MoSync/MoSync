#ifndef _EVENT_H_
#define _EVENT_H_

#include <ma.h>

#define TRUE 1
#define FALSE 0

BOOL left_pressed =FALSE;
BOOL right_pressed =FALSE;
BOOL up_pressed =FALSE;
BOOL down_pressed =FALSE;
BOOL fire_pressed =FALSE;
BOOL pound_pressed =FALSE;
BOOL star_pressed =FALSE;
BOOL zero_pressed =FALSE;

BOOL left =FALSE;
BOOL right =FALSE;
BOOL up =FALSE;
BOOL down =FALSE;
BOOL fire =FALSE;
BOOL quit =FALSE;
BOOL updated =FALSE;
BOOL pound =FALSE;
BOOL star =FALSE;
BOOL zero =FALSE;

void EventHandler_updateEvents() {
	int event;
	left_pressed = right_pressed = up_pressed = down_pressed = zero_pressed = fire_pressed = pound_pressed = star_pressed = FALSE;
	updated = FALSE;
	/// update key states
	while((event = maGetEvent())!=0) {
		updated = TRUE;
		switch(event) {
				case MAK_FIRE:
					fire = TRUE;
					fire_pressed = TRUE;
					break;
				case MAK_LEFT:
					left = TRUE;
					left_pressed = TRUE;
					break;
				case MAK_RIGHT:
					right = TRUE;
					right_pressed = TRUE;
					break;
				case MAK_UP:
					up = TRUE;
					up_pressed = TRUE;
					break;
				case MAK_DOWN:
					down = TRUE;
					down_pressed = TRUE;
					break;
				case MAK_POUND:
				case MAK_1:
					pound = TRUE;
					pound_pressed = TRUE;
					break;
				case MAK_STAR:
				case MAK_3:
					star = TRUE;
					star_pressed = TRUE;
					break;
				case MAK_0:
				case MAK_2:
					zero = TRUE;
					zero_pressed = TRUE;
					break;
				case MAK_FIRE|MAKE_RELEASED:
					fire = FALSE;
					break;
				case MAK_LEFT|MAKE_RELEASED:
					left = FALSE;
					break;
				case MAK_RIGHT|MAKE_RELEASED:
					right = FALSE;
					break;
				case MAK_UP|MAKE_RELEASED:
					up = FALSE;
					break;
				case MAK_DOWN|MAKE_RELEASED:
					down = FALSE;
					break;
				case MAK_POUND|MAKE_RELEASED:
				case MAK_1|MAKE_RELEASED:
					pound = FALSE;
					break;
				case MAK_STAR|MAKE_RELEASED:
				case MAK_3|MAKE_RELEASED:
					star = FALSE;
					break;
				case MAK_0|MAKE_RELEASED:
				case MAK_2|MAKE_RELEASED:
					zero = FALSE;
					break;
				case EVENT_CLOSE:
					quit = TRUE;
					break;
		}
	}
}

#endif // _EVENT_H_
