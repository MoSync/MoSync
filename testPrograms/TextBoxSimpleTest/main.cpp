/* Copyright (C) 2010 MoSync AB

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
#include <conprint.h>

/**
 * Simple use case for the text box.
 * This program will show a text box and
 * retrieve the text entered by the user.
 */
extern "C" int MAMain() {
	// Initializations
	const wchar* title = L"My Title";
	const wchar* text = L"My Text";
	const int length = 1024;
	wchar buf[length];

	// Shows the text box
	// Returns when the text box is ready
	// to receive input
	maTextBox(title, text, buf, length, 0);

	// Event loop
	MAEvent event;
	maWait(0);
	while(maGetEvent(&event))
	{
		// Exit when the fire key is pressed
		if(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_FIRE) {
			maExit(0);
		}
		// When the TextBox closes, display the result (OK or CANCEL),
		// the text retrieved, and its length
		if( event.type == EVENT_TYPE_TEXTBOX ) {
			printf("Result (OK = 1 ; CANCEL = 2): %d\n", event.textboxResult);
			wprintf(L"The text box contained: %s\n", buf);
			printf("Length of the text: %d\n", event.textboxLength);
			printf("Press fire to exit...\n");
		}
		maWait(0);
	}
	return 0;
}

