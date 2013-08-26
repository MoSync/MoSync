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

#include <conprint.h>
#include <maassert.h>
#include <IX_WLAN.H>

void dumpAllAvailableAps() {
	MAWlanAp ap;
	char name[128];
	ap.name = name;
	ap.nameBufSize = 128;
	
	while(maWlanGetNewAp(&ap) > 0) {
		byte* mac = ap.address.a;
		printf("%02X%02X%02X%02X%02X%02X %i dBm\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
			ap.signalStrength);
		printf("SSID: '%s'\n", ap.name);
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
		MAEvent event;
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
