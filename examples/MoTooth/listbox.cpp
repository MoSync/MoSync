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

#include "MoTooth.h"
#include "listbox.h"

void fillScreen() {
	MAExtent s = maGetScrSize();
	maFillRect(0, 0, EXTENT_X(s), EXTENT_Y(s));
}

//returns true if user selected a service, false otherwise.
bool doSelect(int& devIndex, int& servIndex) {
	devIndex = 0;
	servIndex = 0;
	int origServIndex=0, origDevIndex=0;

	//jump over zero-service devices
	MAASSERT(gDevices.size() > 0);
	MAASSERT(gnServices > 0);

	bool down = true;

	for(;;) {
		while(down && gDevices[devIndex].services.size() == 0 && devIndex < gDevices.size() - 1) {
			devIndex++;
			servIndex = 0;
		}
		while(gDevices[devIndex].services.size() == 0 && devIndex > 0) {
			devIndex--;
			servIndex = gDevices[devIndex].services.size() - 1;
		}
		if(gDevices[devIndex].services.size() == 0) {
			servIndex = origServIndex;
			devIndex = origDevIndex;
		}

		drawBox(devIndex, servIndex);

		MAEvent event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE)
				maExit(0);
			if(event.type == EVENT_TYPE_KEY_PRESSED) {
				if(event.key == MAK_DOWN) {
					down = true;
					servIndex++;
					if(servIndex >= gDevices[devIndex].services.size()) {
						if(devIndex < gDevices.size() - 1) {
							devIndex++;
							servIndex = 0;
						} else {
							servIndex--;
						}
					}
				}
				if(event.key == MAK_UP) {
					down = false;
					origServIndex = servIndex;
					origDevIndex = devIndex;
					servIndex--;
					if(servIndex < 0) {
						if(devIndex > 0) {
							devIndex--;
							servIndex = gDevices[devIndex].services.size() - 1;
						} else {
							servIndex++;
						}
					}
				}
				if(event.key == MAK_FIRE)
					return true;
				if(event.key == MAK_0)
					return false;
			}
		}	//while(event = maGetEvent())
	}	//for(;;)

	return true;
}


//draw devices and services. ex:
//MS-FREDRIK
// 0123456789ab
// port 29
// port 30
//HOMUX GPSplim236
// fedcba987654
// port 2

//one service is marked with different background color.
//TODO: draw scrollbar, if needed, on right side of screen, on top of all other graphics.
void drawBox(int devIndex, int servIndex) {
	int line = 0;
	int textHeight = EXTENT_Y(maGetTextSize(" "));
	MAExtent scrSize = maGetScrSize();
	int screenWidth = EXTENT_X(scrSize);
	int screenHeight = EXTENT_Y(scrSize);
	char buffer[270];
	bool drawnSelected = false;

	maSetColor(BLACK);
	fillScreen();

	for(int i=0; i<gDevices.size(); i++) {
		DEVICE& dev = gDevices[i];

		maSetColor(WHITE);
		
		if((line + 2 + dev.services.size()) * textHeight > screenHeight) {
			if(!drawnSelected) {
				line = 0;
				maSetColor(BLACK);
				fillScreen();
				maSetColor(WHITE);
			} else
				break;
		}

		sprintf(buffer, "%s", dev.name.c_str());
		maDrawText(0, line++ * textHeight, buffer);

		sprintf(buffer, " %s", btaddr2string(dev.address));
		maDrawText(0, line++ * textHeight, buffer);

		for(int j=0; j<dev.services.size(); j++) {
			SERVICE& serv = dev.services[j];
			if(i == devIndex && j == servIndex) {
				maSetColor(SKY_BLUE);
				maFillRect(0, line * textHeight, screenWidth, textHeight);
				maSetColor(WHITE);
				drawnSelected = true;
			}
			sprintf(buffer, " port %i: %s", serv.port, serv.name.c_str());
			maDrawText(0, line++ * textHeight, buffer);
		}
	}
	maUpdateScreen();
}
