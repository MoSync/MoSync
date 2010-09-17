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

