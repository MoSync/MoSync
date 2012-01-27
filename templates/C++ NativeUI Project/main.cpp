#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

using namespace MAUtil;
using namespace NativeUI;

#define PRESS_ME_MESSAGE "Press me"
#define HELLO_WORLD_MESSAGE "Hello World!"

/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet : public Moblet, public ButtonListener
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
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		mButton->removeButtonListener(this);
		// All the children will be deleted.
		delete mScreen;
	}

	/**
	 * Create the user interface.
	 */
	void createUI()
	{
		// Create a NativeUI screen that will hold layout and widgets.
		mScreen = new Screen();

		//Create a Vertical Layout that will hold widgets
		mLayout = new VerticalLayout();

		//Let the layout fill the entire screen
		mLayout->fillSpaceHorizontally();
		mLayout->fillSpaceVertically();


		//Create a Native UI button
		mButton = new Button();

		//Let the button fill it's layout
		mButton->fillSpaceHorizontally();
		mButton->fillSpaceVertically();

		//Set the moblet to receive events from the button
		mButton->addButtonListener(this);

		//Set the text of the button
		mButton->setText(PRESS_ME_MESSAGE);

		//Add the button to the layout
		mLayout->addChild(mButton);

		//Add the layout to the screen
		mScreen->setMainWidget(mLayout);

		//Show the screen
		mScreen->show();

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
	* This method is called when there is an touch-down event for
	* a button.
	* Only for iphone platform.
	* @param button The button object that generated the event.
	*/
	virtual void buttonPressed(Widget* button)
	{
		((Button*) button)->setText(HELLO_WORLD_MESSAGE);
	};

	/**
	* This method is called when there is an touch-up event for
	* a button.
	* Only for iphone platform.
	* @param button The button object that generated the event.
	*/
	virtual void buttonReleased(Widget* button)
	{
		((Button*) button)->setText(PRESS_ME_MESSAGE);
	};

	/**
	* This method is called if the touch-up event was inside the
	* bounds of the button.
	* @param button The button object that generated the event.
	*/
	virtual void buttonClicked(Widget* button)
	{
	}

private:
    Screen* mScreen;			//A Native UI screen
    VerticalLayout* mLayout;	//A Native UI layout
    Button* mButton;			//A Native UI button
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new NativeUIMoblet());
	return 0;
}
