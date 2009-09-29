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

#define MAK_0 48
#define EVENT_TYPE_CLOSE 1
#define EVENT_TYPE_KEY_PRESSED 2

typedef struct EVENT {
	int type;
	int key;
} EVENT;

int maSetColor(int rgb);
void maDrawText(int left, int top, const char* str);
void maUpdateScreen();
int maGetEvent(EVENT* event);
void maWait(int timeout);

int MAMain() {
	maSetColor(0xFFFFFF);
	maDrawText(0,0, "Hello World!");
	maUpdateScreen();

	//Freeze
	while(1) {
		maWait(0);
		EVENT event;
		if(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE)
				break;
			if(event.type == EVENT_TYPE_KEY_PRESSED) {
				if(event.key == MAK_0)
					break;
			}
		}
	}
	return 0;

}
