#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <IX_WIDGET.h>
#include <MAUtil/Vector.h>
#include <NativeUI/Widgets.h>

using namespace MAUtil;
using namespace NativeUI;

const char* sWidgetStrings[] =
{
		"ActivityIndicator",
		"Button",
		"CameraPreview",
		"CheckBox",
		"DatePicker",
		"EditBox",
		"GLView",
		"GL2View",
		"HorizontalLayout",
		"VerticalLayout",
		"Image",
		"ImageButton",
		"Label",
		"ListView",
		"ListViewItem",
		"NavBar",
		"NumberPicker",
		"ProgressBar",
		"RelativeLayout",
		"Screen",
		"SearchBar",
		"Slider",
		"StackScreen",
		"TabScreen",
		"TimePicker",
		"ToggleButton",
		"VideoView",
		"WebView"
};
const int sNumWidgets = sizeof(sWidgetStrings)/sizeof(const char*);

/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet : public Moblet, public ButtonListener
{
private:
	Button* mCreateButtons[sNumWidgets];
	Button* mDestroyButtons[sNumWidgets];
	Vector<MAWidgetHandle> mCreatedWidgets[sNumWidgets];
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
		Screen* screen = new Screen();
		VerticalLayout* layout = new VerticalLayout();
		layout->setProperty("isScrollable", "true");
		for(int i = 0; i < sNumWidgets; i++) {
			mCreateButtons[i] = new Button();
			mCreateButtons[i]->fillSpaceHorizontally();
			mCreateButtons[i]->wrapContentVertically();
			mCreateButtons[i]->setText(String("+ ") + sWidgetStrings[i]);
			mDestroyButtons[i] = new Button();
			mDestroyButtons[i]->fillSpaceHorizontally();
			mDestroyButtons[i]->wrapContentVertically();
			mDestroyButtons[i]->setText(String("- ") + sWidgetStrings[i]);

			HorizontalLayout *element = new HorizontalLayout();
			element->fillSpaceHorizontally();
			element->wrapContentVertically();
			element->addChild(mCreateButtons[i]);
			element->addChild(mDestroyButtons[i]);
			layout->addChild(element);

			mCreateButtons[i]->addButtonListener(this);
			mDestroyButtons[i]->addButtonListener(this);

		}


		screen->setMainWidget(layout);

		screen->show();
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

	void buttonClicked(Widget* button) {
		for(int i = 0; i < sNumWidgets; i++) {

			if(button == mCreateButtons[i]) {
				MAWidgetHandle h = maWidgetCreate(sWidgetStrings[i]);
				if(h < 0)
					maPanic(1, "Failed to create widget!");
				mCreatedWidgets[i].add(h);
			}
			else if(button == mDestroyButtons[i]) {
				if(mCreatedWidgets[i].size()) {
					int numWidgets = mCreatedWidgets[i].size();
					int ret = maWidgetDestroy(mCreatedWidgets[i][numWidgets-1]);
					if(ret < 0)
						maPanic(1, "Failed to destroy widget!");
					mCreatedWidgets[i].resize(numWidgets-1);

				}
			}
		}
	}

private:
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new NativeUIMoblet());
	return 0;
}
