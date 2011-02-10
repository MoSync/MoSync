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
