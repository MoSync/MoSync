#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

using namespace MAUtil;
using namespace NativeUI;

static MAAudioInstance sAudioInstance;

#define BUFFERSIZE 4096
static short sBuffer[BUFFERSIZE];
static int sBufferTime1 = 0;
static int sBufferTime2 = 0;

/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet : public Moblet, public IdleListener
{
public:
	/**
	 * The constructor creates the user interface.
	 */
	NativeUIMoblet()
	{
		createUI();
		sAudioInstance = maAudioInstanceCreateDynamic(44100, 1, BUFFERSIZE);

		maAudioPlay(sAudioInstance);
		Environment::getEnvironment().addIdleListener(this);
	}

	Slider* freq1Slider;
	Slider* freq2Slider;
	Slider* fmAmtSlider;

	void renderBuffer(int numSamples)
	{

		int frequency1 = ((50<<16) + (freq1Slider->getValue())*800.0)/44100.0f;
		int frequency2 = ((50<<16) + (freq2Slider->getValue())*800.0)/44100.0f;

		int fmAmt = (fmAmtSlider->getValue());

		for(int i = 0; i < numSamples; i++)
		{
			int osc2 = (sBufferTime2 & 0xffff)*2 - 32768;
			int freq = frequency1 + ((osc2*fmAmt)>>20);

			int osc1 = (sBufferTime1 & 0xffff) - 32768;
			sBuffer[i] = (short)(osc1);
			sBufferTime1+=freq;
			sBufferTime2+=frequency2;
		}

		maAudioSubmitBuffer(sAudioInstance, sBuffer, numSamples*sizeof(short));
	}

	void idle()
	{
		int pendingBuffers = maAudioGetPendingBufferCount(sAudioInstance);

		if(pendingBuffers < 4)
		{
			for(int i = pendingBuffers; i <= 4; i++)
			{
				renderBuffer(BUFFERSIZE);
			}
		}
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		// All the children will be deleted.
		delete mScreen;
	}

	Slider* addSlider()
	{
		Slider* slider = new Slider();
		slider->fillSpaceHorizontally();
		slider->wrapContentVertically();
		slider->setMaximumValue(65536);
		return slider;
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

		mLayout->addChild((freq1Slider = addSlider()));
		mLayout->addChild((freq2Slider = addSlider()));
		mLayout->addChild((fmAmtSlider = addSlider()));

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

private:
    Screen* mScreen;			//A Native UI screen
    VerticalLayout* mLayout;	//A Native UI layout
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new NativeUIMoblet());
	return 0;
}
