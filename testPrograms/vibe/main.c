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

int MAMain() {
	int available = maVibrate(1);
	int on = FALSE;

	maSetColor(-1);
	maDrawText(0, 0, available ? "Vibrator available" : "Vibrator unavaliable");
	maUpdateScreen();

	for(;;) {
		EVENT event;
		while(maGetEvent(&event)) {
			switch(event.type) {
				case EVENT_TYPE_KEY_PRESSED:
					switch(event.key) {
				case MAK_FIRE:
					if(!on) {
						on = TRUE;
						if(!maVibrate(1000))
							maDrawText(0, 100, "Error!");
					} else {
						on = FALSE;
						maVibrate(0);
					}
					maSetColor(0);
					maFillRect(0, 50, EXTENT_X(maGetScrSize()), 50);
					maSetColor(-1);
					maDrawText(0, 50, on ? "Vibrator on" : "Vibrator off");
					maUpdateScreen();
					break;
				case MAK_0:
					maExit(0);
					break;
					}
					break;
				case EVENT_TYPE_CLOSE:
					maExit(0);
					break;
			}
		}
		if(on) {
			if(!maVibrate(1000))
				maDrawText(0, 100, "Error!");
			maWait(40);
		} else {
			maWait(0);
		}
	}
}
