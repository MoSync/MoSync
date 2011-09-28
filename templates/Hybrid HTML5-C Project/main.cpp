#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <NativeUI/Screen.h>
#include <NativeUI/WebView.h>
#include <NativeUI/WebViewListener.h>
#include <NativeUI/WebViewMessage.h>
#include <NativeUI/FileUtil.h>

using namespace MAUtil;
using namespace NativeUI;

class MyWebViewListener : public WebViewListener
{
    virtual void webViewHookInvoked(
		WebView* webView,
		int hookType,
		MAHandle urlData)
    {
         maDestroyObject(urlData);
    }

    virtual void webViewContentLoading(
		WebView* webView,
		const int webViewState)
    {
    }
};

/**
 * A Moblet is a high-level class that defines the
 * behaviour of a MoSync program.
 */
class MyMoblet : public Moblet
{
public:
	/**
	 * Initialize the application in the constructor.
	 */
	MyMoblet()
	{
		extractFileSystem();
		createUI();
	}

	void extractFileSystem()
	{
		FileUtil* fileUtil = FileUtil::create();
		fileUtil->extractLocalFiles();
		delete fileUtil;
	}

	void createUI()
	{
		Screen* screen = new Screen();
		WebView* webView = new WebView();
		webView->fillSpaceHorizontally();
		webView->fillSpaceVertically();
		webView->enableWebViewMessages();
		webView->openURL("index.html");
		screen->setMainWidget(webView);
		screen->show();
	}

	/**
	 * This method is called when a key is pressed, and closes
	 * the application when the back key (on Android) is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
