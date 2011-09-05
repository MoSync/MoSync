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
	NativeUIMoblet()
	{
		createUI();
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
			maPanic(0,
				"NativeUI is only available on Android and iPhone. "
				"You must run directly on the device or devices emulator.");
		}

		// Create a list view.
		mList = maWidgetCreate(MAW_LIST_VIEW);
		widgetSetPropertyInt(
				mList,
			MAW_WIDGET_WIDTH,
			MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				mList,
			MAW_WIDGET_HEIGHT,
			MAW_CONSTANT_WRAP_CONTENT);

		maWidgetAddChild(screen, mList);

		//Set a font size. This is independent of screen size.
		int fontSize = 20;

		//Get the number of fonts installed in the device
		int mNumFonts = maFontGetCount();

		//Set a buffer for font names
		char buffer[64];

		for (int i = 0; i < mNumFonts; i++)
		{
			//Get the i-th font name
			if (maFontGetName(i, buffer, 64))
			{
				//Load the font, and get it's handle
				MAHandle font = maFontLoadWithName(buffer, fontSize);

				//Create a list item
				MAHandle listItem = maWidgetCreate(MAW_LIST_VIEW_ITEM);

				widgetSetPropertyInt(
						listItem,
					MAW_WIDGET_WIDTH,
					MAW_CONSTANT_FILL_AVAILABLE_SPACE);
				widgetSetPropertyInt(
						listItem,
					MAW_WIDGET_HEIGHT,
					MAW_CONSTANT_WRAP_CONTENT);

				//Create a label that goes in the list item
				MAHandle listLabel = maWidgetCreate(MAW_LABEL);
				widgetSetPropertyInt(
						listLabel,
					MAW_WIDGET_WIDTH,
					MAW_CONSTANT_FILL_AVAILABLE_SPACE);
				widgetSetPropertyInt(
						listLabel,
					MAW_WIDGET_HEIGHT,
					MAW_CONSTANT_WRAP_CONTENT);

				//Set the label text as the font name
				maWidgetSetProperty(
						listLabel,
						MAW_LABEL_TEXT,
						buffer);

				//Set the font of the label as the font we just loaded
				widgetSetPropertyInt(
						listLabel,
						"fontHandle",
						font);

				maWidgetAddChild(listItem, listLabel);

				maWidgetAddChild(mList, listItem);
			}
		}

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

		}
	}

	/**
	 * Helper method for setting a widget property integer value.
	 */
	int widgetSetPropertyInt(MAHandle handle, const char *property, int value)
	{
		char buffer[256];
		sprintf(buffer, "%i", value);
		maWidgetSetProperty(handle, property, buffer);
	}

private:
	MAHandle mList;

};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new NativeUIMoblet());
	return 0;
}
