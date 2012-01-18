#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>
#include <ResCompiler/ResCompiler.h>

#include "MAHeaders.h"

using namespace MAUtil;
using namespace NativeUI;

#define PLATFORM_TEXT "Platform"
#define SCREEN_TEXT "Screen"

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
		mPlatformButton->removeButtonListener(this);
		mScreenButton->removeButtonListener(this);
		// All the children will be deleted.
		delete mMainScreen;
	}

	/**
	 * Create the user interface.
	 */
	void createMainUI()
	{
		// Create a NativeUI screen that will hold layout and widgets.
		mMainScreen = new Screen();

		//Create a Vertical Layout that will hold widgets
		mMainLayout = new VerticalLayout();

		//Let the layout fill the entire screen
		mMainLayout->fillSpaceHorizontally();
		mMainLayout->fillSpaceVertically();


		//Create a Native UI button
		mPlatformButton = new Button();

		//Let the button fill it's layout
		mPlatformButton->fillSpaceHorizontally();
		//mButton->fillSpaceVertically();

		//Set the moblet to receive events from the button
		mPlatformButton->addButtonListener(this);

		//Set the text of the button
		mPlatformButton->setText(PLATFORM_TEXT);

		//Add the button to the layout
		mMainLayout->addChild(mPlatformButton);

		//Create a Native UI button
		mScreenButton = new Button();

		//Let the button fill it's layout
		mScreenButton->fillSpaceHorizontally();

		//Set the moblet to receive events from the button
		mScreenButton->addButtonListener(this);

		//Set the text of the button
		mScreenButton->setText(SCREEN_TEXT);

		//Add the button to the layout
		mMainLayout->addChild(mScreenButton);

		//Add the layout to the screen
		mMainScreen->setMainWidget(mMainLayout);

		//Show the screen
		mMainScreen->setVisible(true);
		mMainScreen->show();

	}

	void createPlatformUI()
	{
		mPlatformScreen = new Screen();
		mPlatformScreen->fillSpaceHorizontally();
		mPlatformScreen->fillSpaceVertically();

		mPlatformLayout = new RelativeLayout();
		mPlatformLayout->fillSpaceHorizontally();
		mPlatformLayout->fillSpaceVertically();

		loadResource(PLATFORM);
		mPlatformImage = new Image();
		mPlatformImage->setBackgroundColor(0xFFFFFF);
		mPlatformImage->fillSpaceHorizontally();
		mPlatformImage->fillSpaceVertically();
		mPlatformImage->setImage(PLATFORM);

		mPlatformLayout->addChild(mPlatformImage);
		mPlatformScreen->addChild(mPlatformLayout);
	}

	void createScreenUI()
	{
		mScreenScreen = new Screen();
		mScreenScreen->fillSpaceHorizontally();
		mScreenScreen->fillSpaceVertically();

		mScreenLayout = new RelativeLayout();
		mScreenLayout->fillSpaceHorizontally();
		mScreenLayout->fillSpaceVertically();

		loadResource(SCREEN_TYPE);
		mScreenImage = new Image();
		mScreenImage->setBackgroundColor(0xFFFFFF);
		mScreenImage->fillSpaceHorizontally();
		mScreenImage->fillSpaceVertically();
		mScreenImage->setImage(SCREEN_TYPE);

		mScreenLayout->addChild(mScreenImage);
		mScreenScreen->addChild(mScreenLayout);
	}

	void createUI()
	{
		createMainUI();
		createPlatformUI();
		createScreenUI();
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			if (mMainScreen->isVisible())
			{
				// Call close to exit the application.
				close();
			}
			else
			{
				mMainScreen->setVisible(true);
				mMainScreen->show();
			}
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
	};

	/**
	* This method is called when there is an touch-up event for
	* a button.
	* Only for iphone platform.
	* @param button The button object that generated the event.
	*/
	virtual void buttonReleased(Widget* button)
	{
	};

	/**
	* This method is called if the touch-up event was inside the
	* bounds of the button.
	* @param button The button object that generated the event.
	*/
	virtual void buttonClicked(Widget* button)
	{
		if (button == mPlatformButton)
		{
			mPlatformScreen->show();
			mMainScreen->setVisible(false);
		}
		else if (button == mScreenButton)
		{
			mScreenScreen->show();
			mMainScreen->setVisible(false);
		}
	}

	void pointerPressEvent (MAPoint2d p)
	{
		if (!mMainScreen->isVisible())
		{
			mMainScreen->setVisible(true);
			mMainScreen->show();
		}
	}

private:
    Screen* mMainScreen;			//A Native UI screen
    VerticalLayout* mMainLayout;	//A Native UI layout
    Button* mPlatformButton;		//A Native UI button
    Button* mScreenButton;			//A Native UI button

    Screen* mPlatformScreen;
    RelativeLayout* mPlatformLayout;
    Image* mPlatformImage;

    Screen* mScreenScreen;
    RelativeLayout* mScreenLayout;
    Image* mScreenImage;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	printf("MAMain");
	Moblet::run(new NativeUIMoblet());
	return 0;
}
