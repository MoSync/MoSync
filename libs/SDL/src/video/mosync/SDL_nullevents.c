/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

/* Being a null driver, there's no event stream. We just define stubs for
   most of the API. */

#include "SDL.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"

#include "SDL_nullvideo.h"
#include "SDL_nullevents_c.h"

#include <IX_AUDIOBUFFER.h>
#include <ma.h>

enum MOUSE_ACTION { MOUSE_DOWN = 0, MOUSE_UP=1, MOUSE_MOVE=2 };

void SDL_MoSync_AudioBufferFill();

void DUMMY_PumpEvents(_THIS)
{
	MAEvent event;
	SDL_keysym ks;

	while(maGetEvent(&event)) {
		switch(event.type) {
			case EVENT_TYPE_AUDIOBUFFER_FILL:
				SDL_MoSync_AudioBufferFill();
				break;
			case EVENT_TYPE_CLOSE:
				//if(SDL_PrivateQuit()) {
					// must do some kind of delayed quit..¨
					maExit(0);
				//}
				break;
			case EVENT_TYPE_POINTER_PRESSED:
			case EVENT_TYPE_POINTER_RELEASED:
			case EVENT_TYPE_POINTER_DRAGGED:
				SDL_PrivateMouseMotion(0, 0, event.point.x, event.point.y);
				if(event.type==EVENT_TYPE_POINTER_PRESSED) SDL_PrivateMouseButton(SDL_PRESSED, 1, 0, 0 );
				if(event.type==EVENT_TYPE_POINTER_RELEASED) SDL_PrivateMouseButton(SDL_RELEASED, 1, 0, 0 );
				break;

			case EVENT_TYPE_KEY_PRESSED:
			case EVENT_TYPE_KEY_RELEASED:
				ks.scancode = event.nativeKey;
				ks.sym = event.key;
				ks.mod = KMOD_NONE;
				SDL_PrivateKeyboard( event.type==EVENT_TYPE_KEY_PRESSED?SDL_PRESSED:SDL_RELEASED, &ks);
				break;
		}
	}
	/* do nothing. */
}

void DUMMY_InitOSKeymap(_THIS)
{
	/* do nothing. */
}

/* end of SDL_nullevents.c ... */

