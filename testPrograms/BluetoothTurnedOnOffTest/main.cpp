/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
