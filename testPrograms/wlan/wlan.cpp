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

#include <conprint.h>
#include <maassert.h>
#include "IX_WLAN.H"

void dumpAllAvailableAps() {
	MA_WLAN_AP ap;
	char name[128];
	ap.name = name;
	ap.nameBufSize = 128;
	
	while(maWlanGetNewAp(&ap) > 0) {
		byte* mac = ap.address.a;
		printf("%02X%02X%02X%02X%02X%02X %i dBm\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
			ap.signalStrength);
	}
}

extern "C" int MAMain() {
	int discoveryState;
	InitConsole();
	gConsoleLogging = 1;

	printf("Hello World!\n");
	discoveryState = maWlanStartDiscovery();
	printf("WLAN: %i\n", discoveryState);
	
	while(1) {
		maWait(0);
		EVENT event;
		if(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE)
				break;
			if(event.type == EVENT_TYPE_KEY_PRESSED) {
				if(event.key == MAK_0)
					break;
				if(event.key == MAK_FIRE && discoveryState == 1) {
					discoveryState = maWlanStartDiscovery();
					printf("WLAN: %i\n", discoveryState);
				}
			}
			if(event.type == EVENT_TYPE_WLAN) {
				printf("State %i\n", event.state);
				dumpAllAvailableAps();
				printf("Done.\n");
				discoveryState = event.state;
			}
		}
	}
	return 0;
}
