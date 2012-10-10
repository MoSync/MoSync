/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file HybridMoblet.cpp
 * @author Mikael Kindborg
 *
 * @brief High-level moblet that has a WebView and supports
 * communication between a JavaScript and C++.
 */

#include <mastdlib.h>
#include <mavsprintf.h>
#include <NativeUI/WebViewListener.h>
#include "HybridMoblet.h"

using namespace MAUtil;

namespace Wormhole
{

/**
 * Class that listens for WebView events.
 * Here we receive messages from JavaScript.
 */
class HybridMoblet_WebViewListener : public NativeUI::WebViewListener
{
public:
	/**
	 * Constructor that saves the pointer to the moblet.
	 */
	HybridMoblet_WebViewListener(HybridMoblet* moblet)
	{
		mMoblet = moblet;
	}

	/**
	 * This method is called when a "mosync://" url is
	 * invoked in the WebView.
	 *
	 * Note: The urlData is deallocated automatically
	 * by the WebView after this call finishes.
	 */
	virtual void webViewHookInvoked(
		NativeUI::WebView* webView,
		int hookType,
		MAHandle urlData)
	{
		// Note: urlData is deallocated automatically by
		// the framework, we should not deallocate it here.

		mMoblet->handleWebViewMessage(webView, urlData);
	}

private:
	/**
	 * Pointer to the moblet.
	 */
	HybridMoblet* mMoblet;
};
// End of class HybridMoblet_WebViewListener

// ***** Methods for class HybridMoblet ***** //

/**
 * Constructor.
 */
HybridMoblet::HybridMoblet()
{
	// I don't care about using initialisers!
	mInitialized = false;
	mWebView = NULL;

	// Create file utility object.
	mFileUtil = new FileUtil();
}

/**
 * Destructor.
 */
HybridMoblet::~HybridMoblet()
{
	// Deleting the root widget will also delete child widgets.
	delete mScreen;

	// Delete the WebView listener.
	if (NULL != mWebViewListener)
	{
		delete mWebViewListener;
		mWebViewListener = NULL;
	}

	// Delete the file utility object.
	delete mFileUtil;
}

void HybridMoblet::initialize()
{
	if (mInitialized) { return; }

	mInitialized = true;

	// Extract files in LocalFiles folder to the device.
	extractFileSystem();

	// Enable message sending from JavaScript to C++.
	enableWebViewMessages();

	// Initialize the message handler. All messages from
	// JavaScript are routed through this handler.
	mMessageHandler.initialize(this);
}

void HybridMoblet::openWormhole(
	Wormhole::HybridMoblet* moblet)
{
	mMessageHandler.openWormhole(moblet);
}

/**
 * Get the WebView widget displayed by this moblet.
 * Creates the WebView if it does not exist.
 * @return Pointer to a WebView instance.
 */
NativeUI::WebView* HybridMoblet::getWebView()
{
	// Create the WebView if it does not exist.
	if (NULL == mWebView)
	{
		// Create and configure the WebView.
		mWebView = new NativeUI::WebView();
		mWebView->fillSpaceHorizontally();
		mWebView->fillSpaceVertically();

		// Create the screen that holds the WebView.
		mScreen = new NativeUI::Screen();
		mScreen->setMainWidget(mWebView);
	}

	return mWebView;
}

/**
 * Get a file utility object used for accessing the
 * device's local file system.
 * @return Pointer to a FileUtil instance.
 */
FileUtil* HybridMoblet::getFileUtil()
{
	return mFileUtil;
}

/**
 * Display a page in the WebView of this moblet.
 * @param url Url of page to open.
 */
void HybridMoblet::showPage(const MAUtil::String& url)
{
	// Extract files system and perform other initialisation.
	initialize();

	// Make the main WebView visible.
	getWebView()->setVisible(true);

	// Show the WebView screen.
	showWebView();

	// Open the page.
	getWebView()->openURL(url);
}

/**
 * Display a NativeUI page.
 * @param url Url of NativeUI page to open.
 */
void HybridMoblet::showNativeUI(const MAUtil::String& url)
{
	// Extract files system and perform other initialisation.
	initialize();

	// In a NativeUI app we hide the main WebView and use NativeUI
	// to display widgets.
	getWebView()->setVisible(false);

	// Open the page.
	getWebView()->openURL(url);
}

/**
 * Display the main WebView of this moblet.
 */
void HybridMoblet::showWebView()
{
	// Make sure the WebView is created.
	getWebView();

	// Show the screen that contains the WebView.
	mScreen->show();
}

/**
 * Set the sound used for the PhoneGap beep notification.
 *
 * @param beepSoundResource Handle to the "beep" sound
 * played by the PhoneGap API.
 */
void HybridMoblet::setBeepSound(MAHandle beepSoundResource)
{
	mMessageHandler.setBeepSound(beepSoundResource);
}

/**
 * Add a function invoked when a message is sent from JavaScript.
 *
 * Invoke from JavaScript using this format:
 *
 *   mosync.bridge.send(["Custom", "Vibrate"]);
 *
 * First parameter must always be "Custom". Then the string
 * (command name) used to identify the function. Optionally
 * you can send additional parameters and retrieve them
 * from the message stream.
 *
 * @param command String identifying the command.
 * @param fun Function of type MessageHandlerFun. This function must
 * be a member of a subclass of the class FunObject (HybridMoblet
 * inherits FunObject, methods in subclasses of HybridMoblet work fine).
 */
void HybridMoblet::addMessageFun(
	const char* command,
	FunTable::MessageHandlerFun fun)
{
	mMessageHandler.addMessageFun(command, fun);
}

/**
 * This method handles messages sent from the WebView.
 *
 * Note that the data object will be valid only during
 * the life-time of the call of this method, then it
 * will be deallocated.
 *
 * @param webView The WebView that sent the message.
 * @param urlData Data object that holds message content.
 */
void HybridMoblet::handleWebViewMessage(
	NativeUI::WebView* webView,
	MAHandle data)
{
	mMessageHandler.handleWebViewMessage(webView, data, this);
}

/**
 * This method is called when a key is pressed.
 * Forwards the event to PhoneGapMessageHandler.
 * Override to implement your own behaviour.
 */
void HybridMoblet::keyPressEvent(int keyCode, int nativeCode)
{
	mMessageHandler.keyPressEvent(keyCode, nativeCode);
}

/**
 * Run JavaScript code in the main WebView
 * of this moblet.
 * @param script JavaScript code to evaluate.
 */
void HybridMoblet::callJS(const MAUtil::String& script)
{
	getWebView()->callJS(script);
}

/**
 * Evaluate JavaScript code in a WebView.
 * @param webViewHandle The MoSync handle to the WebView in which
 * to evaluate the script (this handle is an integer id).
 * @param script JavaScript string.
 */
void HybridMoblet::callJS(
	MAWidgetHandle webViewHandle,
	const MAUtil::String& script)
{
	// Call the JavaScript code on the WebView.
	MAUtil::String url = "javascript:" + script;
	maWidgetSetProperty(
		webViewHandle,
		MAW_WEB_VIEW_URL,
		url.c_str());
}

/**
 * Enable JavaScript to C++ communication.
 */
void HybridMoblet::enableWebViewMessages()
{
	if (NULL == mWebViewListener)
	{
		mWebViewListener = new HybridMoblet_WebViewListener(this);
		getWebView()->addWebViewListener(mWebViewListener);
		getWebView()->enableWebViewMessages();
	}
}

/**
 * Disable JavaScript to C++ communication.
 */
void HybridMoblet::disableWebViewMessages()
{
	if (NULL != mWebViewListener)
	{
		getWebView()->removeWebViewListener(mWebViewListener);
		getWebView()->disableWebViewMessages();
		delete mWebViewListener;
		mWebViewListener = NULL;
	}
}

/**
 * Extract HTML/CSS/JS/Media files to the local file system.
 */
void HybridMoblet::extractFileSystem()
{
	// You can display splash screen if this is the
	// first time launch or if the checksum has changed.
	// TODO: Add library support for this? At least
	// document how to do it yourself.
	if (checksumHasChanged())
	{
		// Extract bundled files to the local file system.
		mFileUtil->extractLocalFiles();

		// Write the current checksum.
		writeChecksum();
	}
}

/**
 * @return true if the checksum has changed (or if the old
 * value did not exist, such as on first time load).
 */
bool HybridMoblet::checksumHasChanged()
{
	// Assume checksum has changed (or does not exist).
	bool hasChanged = true;

	// Checksum file path.
	MAUtil::String filePath = getFileUtil()->getLocalPath();
	filePath += "MoSyncFileBundleChecksum";

	// Read checksum of the file system bundle.
	int checksum = getFileUtil()->getFileSystemChecksum(1);

	// REad checksum from file and compare.
	MAUtil::String data;
	if (getFileUtil()->readTextFromFile(filePath, data))
	{
		// Read from file succeeded. Compate values.
		int existingChecksum = (int)strtol(data.c_str(), NULL, 10);
		hasChanged = checksum != existingChecksum;
	}

	return hasChanged;
}

/**
 * Write the current checksum to file.
 */
void HybridMoblet::writeChecksum()
{
	// Get checksum of the file system bundle.
	int checksum = getFileUtil()->getFileSystemChecksum(1);

	// Checksum file path.
	MAUtil::String filePath = getFileUtil()->getLocalPath();
	filePath += "MoSyncFileBundleChecksum";

	// Save checksum value.
	if (checksum != 0)
	{
		char checksumBuf[128];
		sprintf(checksumBuf, "%d", checksum);
		getFileUtil()->writeTextToFile(filePath, checksumBuf);
	}
}

} // namespace
