#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>
#include <MATest/TestRunner.h>

using namespace MATest;
using namespace MAUtil;
using namespace NativeUI;

#define NUM_BUTTONS 100
#define BUTTON_LABEL "Touch Me"

/**
 * Example of NativeUI test.
 */
class TestNativeUI : public TestCase
{
public:
	TestNativeUI(const String& name) : TestCase(name)
	{
	}

	void start()
	{
		createUI();
		testUI();
		runNextTestCase();
	}

	void createUI()
	{
		// Create screen and root layout.
		mScreen = new Screen();
		mLayout = new VerticalLayout();
		mLayout->fillSpaceHorizontally();
		mLayout->fillSpaceVertically();
		mScreen->setMainWidget(mLayout);

		// Create buttons.
		for (int i = 0; i < NUM_BUTTONS; ++i)
		{
			Button* button = new Button();
			button->fillSpaceHorizontally();
			button->wrapContentVertically();
			button->setText(BUTTON_LABEL);
			mLayout->addChild(button);
			mButtons[i] = button;
		}

		// Show the screen.
		mScreen->show();
	}

	void testUI()
	{

		// Last button should exist.
		Button* button = mButtons[NUM_BUTTONS-1];
		assert("LastButtonExists", NULL != button);

		// Last button should have expected text.
		String label = button->getText();
		assert("LastButtonLabel", label == BUTTON_LABEL);

		// Test that buttons are laid out properly.
		// Y coord of last button should be bigger than
		// the width of all buttons except last one.
		int height = button->getPropertyInt(MAW_WIDGET_HEIGHT);
		int top = button->getPropertyInt(MAW_WIDGET_TOP);
		assert("LastButtonLayout", top >= height * (NUM_BUTTONS - 1));

		// Show output screen.
		maWidgetScreenShow(MAW_CONSTANT_MOSYNC_SCREEN_HANDLE);
		delete mScreen;
	}

    Screen* mScreen;
    VerticalLayout* mLayout;
    Button* mButtons[NUM_BUTTONS];
};

TESTCASE(TestNativeUI)
