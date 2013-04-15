#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

using namespace MAUtil;
using namespace NativeUI;

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
		mLayout->setScrollable(true);

		//Create a Native UI button
		mButton = new Button();

		//Let the button fill it's layout
		mButton->fillSpaceHorizontally();
		mButton->fillSpaceVertically();

		//add many buttons and set properties on them in order to test
		//the asynchronously setting of properties on the windows phone runtime
		for (int i = 0; i < 50; i++)
		{
			Button *button = new Button();

			button->setText("Press Me " + MAUtil::integerToString(i));
			button->setText("Press Me " + MAUtil::integerToString(i));
			button->setText("Press Me " + MAUtil::integerToString(i));
			button->setText("Press Me " + MAUtil::integerToString(i));
			button->setText("Press Me " + MAUtil::integerToString(i));

			button->setText("Press Me " + MAUtil::integerToString(i));
			button->setText("Press Me " + MAUtil::integerToString(i));
			button->setText("Press Me " + MAUtil::integerToString(i));
			button->setText("Press Me " + MAUtil::integerToString(i));
			button->setText("Press Me " + MAUtil::integerToString(i));

			mLayout->addChild(button);
		}

		//Set the moblet to receive events from the button
		mButton->addButtonListener(this);

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
	* This method is called if the touch-up event was inside the
	* bounds of the button.
	* @param button The button object that generated the event.
	*/
	virtual void buttonClicked(Widget* button)
	{
		((Button*) button)->setText("Hello World");
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
