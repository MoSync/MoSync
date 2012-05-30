/**
 * Template for a Native UI C application.
 */

#include <ma.h>
#include <mavsprintf.h>
#include <IX_WIDGET.h>

// Reference to button widget.
static MAHandle sButton;

// Number of button clicks.
static int sButtonClickCount = 0;

/**
 * Helper method for setting a widget property integer value.
 */
int widgetSetPropertyInt(MAHandle handle, const char *property, int value)
{
	char buffer[256];
	sprintf(buffer, "%i", value);
	maWidgetSetProperty(handle, property, buffer);
}

/**
 * Create the user interface.
 */
void createUI()
{
	// Create a NativeUI screen that will hold layout and widgets.
	MAHandle screen = maWidgetCreate(MAW_SCREEN);

	// Error handling for devices that do not support NativeUI.
	if (-1 == screen)
	{
		maPanic(0, "NativeUI is only available on Android, iOS, and Windows Phone.");
	}

	// Create main layout.
	MAHandle layout = maWidgetCreate(MAW_VERTICAL_LAYOUT);
	widgetSetPropertyInt(
		layout,
		MAW_WIDGET_WIDTH,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	widgetSetPropertyInt(
		layout,
		MAW_WIDGET_HEIGHT,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	maWidgetSetProperty(
		layout,
		MAW_WIDGET_BACKGROUND_COLOR,
		"0xFFFFFF");
	maWidgetAddChild(screen, layout);

	// Create a button.
	sButton = maWidgetCreate(MAW_BUTTON);
	widgetSetPropertyInt(
		sButton,
		MAW_WIDGET_WIDTH,
		MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	widgetSetPropertyInt(
		sButton,
		MAW_WIDGET_HEIGHT,
		MAW_CONSTANT_WRAP_CONTENT);
	maWidgetSetProperty(
		sButton,
		MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);
	maWidgetSetProperty(
		sButton,
		MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);
	maWidgetSetProperty(
		sButton,
		MAW_BUTTON_TEXT,
		"Click me!");
	maWidgetSetProperty(
		sButton,
		MAW_BUTTON_FONT_COLOR,
		"0x000000");
	maWidgetSetProperty(
		sButton,
		MAW_BUTTON_FONT_SIZE,
		"36");
	maWidgetAddChild(layout, sButton);

	// Show the screen.
	maWidgetScreenShow(screen);
}


/**
 * Function that processes widget events.
 */
void handleWidgetEvent(MAWidgetEventData* widgetEvent)
{
	// Has the button been clicked?
	if (MAW_EVENT_CLICKED == widgetEvent->eventType &&
		sButton == widgetEvent->widgetHandle)
	{
		++sButtonClickCount;
		char buffer[256];
		if (1 == sButtonClickCount)
		{
			sprintf(buffer, "You clicked me once!", sButtonClickCount);
		}
		else if (100 == sButtonClickCount)
		{
			sprintf(buffer, "Wow! 100 times!");
		}
		else
		{
			sprintf(buffer, "You clicked me %i times!", sButtonClickCount);
		}
		maWidgetSetProperty(sButton, "text", buffer);
	}
}

/**
 * The event loop.
 */
void eventLoop()
{
	MAEvent event;

	BOOL isRunning = TRUE;
	while (isRunning)
	{
		maWait(0);
		while (maGetEvent(&event))
		{
			if (EVENT_TYPE_CLOSE == event.type)
			{
				isRunning = FALSE;
				break;
			}
			else if (EVENT_TYPE_KEY_PRESSED == event.type)
			{
				// Exit the app if the back key (on Android) is pressed.
				if (event.key == MAK_BACK)
				{
					isRunning = FALSE;
					break;
				}
			}
			else if (EVENT_TYPE_WIDGET == event.type)
			{
				handleWidgetEvent((MAWidgetEventData*) event.data);
			}
		}
	}
}

/**
 * Main function that is called when the program starts.
 */
int MAMain()
{
	createUI();
	eventLoop();
	return 0;
}
