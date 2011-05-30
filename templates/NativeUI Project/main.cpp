#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <IX_WIDGET.h>

using namespace MAUtil;

/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet : public Moblet
{
public:
	/**
	 * The constructor creates the user interface.
	 */
	NativeUIMoblet() :
		mButtonClickCount(0)
	{
		createUI();
	}

	/**
	 * Create the user interface.
	 */
	void createUI()
	{
		// Create a NativeUI screen that will hold layout and widgets.
		int screen = maWidgetCreate(MAW_SCREEN);

		// Error handling for devices that do not support NativeUI.
		if (-1 == screen)
		{
			maPanic(0, "This program is not supported on this device.");
		}

		// Create a layout object.
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
		mButton = maWidgetCreate(MAW_BUTTON);
		widgetSetPropertyInt(
			mButton,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
			mButton,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);
		maWidgetSetProperty(
			mButton,
			MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT,
			MAW_ALIGNMENT_CENTER);
		maWidgetSetProperty(
			mButton,
			MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT,
			MAW_ALIGNMENT_CENTER);
		maWidgetSetProperty(
			mButton,
			MAW_BUTTON_TEXT,
			"Click me!");
		maWidgetSetProperty(
			mButton,
			MAW_BUTTON_FONT_COLOR,
			"0x000000");
		maWidgetSetProperty(
			mButton,
			MAW_BUTTON_FONT_SIZE,
			"36");
		maWidgetAddChild(layout, mButton);

		// Show the screen.
		maWidgetScreenShow(screen);

		// Make the Moblet listen to widget events.
		Environment::getEnvironment().addCustomEventListener(this);
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

	/**
	 * Method that implements the custom event listener interface.
	 * Widget events are sent as custom events.
	 */
	void customEvent(const MAEvent& event)
	{
		if (EVENT_TYPE_WIDGET == event.type)
		{
			// Get the widget event data structure.
			MAWidgetEventData* widgetEvent = (MAWidgetEventData*) event.data;

			// Has the button been clicked?
			if (MAW_EVENT_CLICKED == widgetEvent->eventType &&
				mButton == widgetEvent->widgetHandle)
			{
				++mButtonClickCount;
				char buffer[256];
				if (1 == mButtonClickCount)
				{
					sprintf(buffer, "You clicked me once!", mButtonClickCount);
				}
				else
				if (100 == mButtonClickCount)
				{
					sprintf(buffer, "Wow! 100 times!", mButtonClickCount);
				}
				else
				{
					sprintf(buffer, "You clicked me %i times!", mButtonClickCount);
				}
				maWidgetSetProperty(mButton, "text", buffer);
			}
		}
	}

	/**
	 * Helper method for setting a widget property interger value.
	 */
	int widgetSetPropertyInt(MAHandle handle, const char *property, int value)
	{
		char buffer[256];
		sprintf(buffer, "%i", value);
		maWidgetSetProperty(handle, property, buffer);
	}

private:
	MAHandle mButton;
	int mButtonClickCount;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new NativeUIMoblet());
	return 0;
}
