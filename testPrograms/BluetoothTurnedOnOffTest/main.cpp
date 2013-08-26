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

#include <ma.h>
#include <conprint.h>

// This program tests Bluetooth on/off events.
// How to use: Start program, then press HOME key on Android,
// turn off/on Bluetooth in the Settings panel.
// Program should print the events.
// Author: Micke

extern "C" int MAMain()
{
	MAEvent event;

	printf("Listening for Bluetooth events");

	while (1)
	{
		maWait(0);
		while (maGetEvent(&event))
		{
			switch (event.type)
			{
				case EVENT_TYPE_CLOSE:
					maExit(0);
					break;
				case EVENT_TYPE_KEY_PRESSED:
					// Exit application on pressing zero or back keys.
					if (event.key == MAK_0 || event.key == MAK_BACK)
					{
						maExit(0);
					}
					break;
				case EVENT_TYPE_BLUETOOTH_TURNED_ON:
					printf("EVENT_TYPE_BLUETOOTH_TURNED_ON");
					break;
				case EVENT_TYPE_BLUETOOTH_TURNED_OFF:
					printf("EVENT_TYPE_BLUETOOTH_TURNED_OFF");
					break;
			}
		}
	}

	return 0;
}
