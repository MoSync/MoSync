#include <MAUtil/Moblet.h>
#include <MAUI/Screen.h>
#include <MAUI/Label.h>

using namespace MAUtil;
using namespace MAUI;

/**
 * MAUI is short for MoSync API User Interface.
 *
 * A Screen is a MAUI object that holds widgets.
 */
class MAUIScreen : public Screen
{
public:
	/**
	 * Create widgets in the constructor.
	 */
	MAUIScreen()
	{
		mBackgroundArea = new Label(
			0, // Left
			0, // Top
			0, // Width
			0, // Height
			NULL // Parent widget
			);
		mBackgroundArea->setBackgroundColor(0xFFFFFF);

		mTouchArea = new Label(
			100, // Left
			100, // Top
			50,  // Width
			50,  // Height
			mBackgroundArea // Parent widget
			);
		mTouchArea->setBackgroundColor(0x000000);

		// Set the main widget of the screen. This will
		// resize the widget to fit the screen.
		setMain(mBackgroundArea);

	}

	/**
	 * Deallocate objects in the destructor.
	 */
	virtual ~MAUIScreen()
	{
		delete mBackgroundArea;
		delete mTouchArea;
	}

	/**
	 * Called when a key is pressed.
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode)
	{
		// Default color is white.
		int color = 0xFFFFFF;

		switch (keyCode)
		{
			case MAK_1:
				color = 0xFF5555;
				break;
			case MAK_2:
				color = 0x55FF55;
				break;
			case MAK_3:
				color = 0x5555FF;
				break;
		}

		// Set the new background color.
		mBackgroundArea->setBackgroundColor(color);
	}

	/**
	 * Called when the screen is touched.
	 */
	virtual void pointerPressEvent(MAPoint2d point)
	{
		// Center the touch area at the pointer position.
		mTouchArea->setPosition(point.x - 25, point.y - 25);

		// We need to repaint the parent when a child
		// widget is changed.
		mBackgroundArea->requestRepaint();
	}

private:
	Label* mBackgroundArea;
	Label* mTouchArea;
};

/**
 * A Moblet is a high-level class that defines the
 * behaviour of a MoSync program.
 *
 * To use MAUI you need a Moblet, but a  Moblet can
 * be used with or without the MAUI library.
 */
class MAUIMoblet : public Moblet
{
public:
	/**
	 * Initialize the application in the constructor.
	 */
	MAUIMoblet()
	{
		mScreen = new MAUIScreen();
		mScreen->show();
	}

	/**
	 * Deallocate objects in the destructor.
	 */
	virtual ~MAUIMoblet()
	{
		delete mScreen;
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

private:
	MAUIScreen* mScreen;
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	Moblet::run(new MAUIMoblet());
	return 0;
}
