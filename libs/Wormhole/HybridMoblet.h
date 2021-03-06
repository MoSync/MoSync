/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

/**
 * @file HybridMoblet.h
 * @author Mikael Kindborg
 *
 * @brief High-level moblet that has a WebView and supports
 * communication between a JavaScript and C++.
 */

#ifndef WORMHOLE_HYBRID_MOBLET_H_
#define WORMHOLE_HYBRID_MOBLET_H_

#include <ma.h>
#include <maassert.h>
#include <MAUtil/Environment.h>
#include <MAUtil/Vector.h>

#include "CustomMoblet.h"
#include "MessageHandler.h"
#include "FileUtil.h"

/**
 * @brief Classes for Hybrid app support.
 */
namespace Wormhole
{
// Forward declaration.
class HybridMoblet_WebViewListener;

/**
 * @brief Moblet designed to make it easy to add custom C++ code that
 * can be invoked from JavaScript. This class provides a
 * Wormhole-enabled WebView, methods to extract and load
 * your HTML files and JavaScript code, and a structure for
 * communicating with JavaScript.
 *
 * Members are made virtual and public by design, to give
 * maximum flexibility to users of this class. Do not access
 * instance variables directly, unless absolutely needed.
 */
class HybridMoblet :
	public CustomMoblet,
	public FunObject
{
public:
	/**
	 * Constructor.
	 */
	HybridMoblet();

	/**
	 * Destructor.
	 */
	virtual ~HybridMoblet();


	/**
	 * Initialize the moblet.
	 */
	virtual void initialize();

	/**
	 * Initialize the Wormhole JS library. Should be
	 * called after the page has been displayed.
	 */
	virtual void openWormhole(MAHandle webViewHandle);

	/**
	 * Creates the main UI elements, but does not connect them.
	 */
	virtual void createUI();

	/**
	 * Get the WebView widget displayed by this moblet.
	 * @return Pointer to the WebView instance.
	 */
	virtual NativeUI::WebView* getWebView();

	/**
	 * Get a file utility object used for accessing the
	 * device's local file system.
	 * @return Pointer to a FileUtil instance.
	 */
	virtual FileUtil* getFileUtil();

	/**
	 * Display a page in the main WebView of this moblet.
	 * Will automatically call initialize() if not called
	 * previously.
	 * @param url Url of page to open.
	 */
	virtual void showPage(const MAUtil::String& url);

	/**
	 * Display a NativeUI page.
	 * @param url Url of NativeUI page to open.
	 */
	virtual void showNativeUI(const MAUtil::String& url);

	/**
	 * Display the main WebView of this moblet.
	 */
	virtual void showWebView();

	/**
	 * Set the sound used for the PhoneGap beep notification.
	 *
	 * @param beepSoundResource Handle to the "beep" sound
	 * played by the PhoneGap API.
	 */
	virtual void setBeepSound(MAHandle beepSoundResource);

	/**
	 * Return the message handler object used by this moblet.
	 */
	virtual MessageHandler* getMessageHandler();

	/**
	 * Set the message handler object used by this moblet.
	 * Any previous handler is deleted. The moblet takes
	 * ownership of the handler and deletes it when destroyed.
	 */
	virtual void setMessageHandler(MessageHandler* handler);

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
	virtual void addMessageFun(
		const char* command,
		Wormhole::FunTable::MessageHandlerFun fun);

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
	virtual void handleWebViewMessage(
		MAWidgetHandle webViewHandle,
		MAHandle data);

	/**
	 * Prints the incoming webview message. Used for debugging.
	 *
	 * To call this method, override HybridMoblet::handleWebViewMessage
	 * in your moblet with the following method:
	 *
	 * void handleWebViewMessage(MAHandle webViewHandle, MAHandle data)
	 * {
	 *    printWebViewMessage(data);
	 *    HybridMoblet::handleWebViewMessage(webViewHandle, data);
	 * }
	 */
	virtual void printWebViewMessage(MAHandle dataHandle);

	/**
	 * Handles HOOK_INVOKED events for WebViews in the app.
	 * This code enables WebViews to send messages to each other.
	 *
	 * The only thing that work reliable from other WebViews than
	 * the main one, are CallJS and calls that do not return anything,
	 * like mosync.app.sendToBackground().
	 *
	 * Apps are supposed to use the main WebView to for accessing the
	 * fulll Wormhole JS API, and only use mosync.nativeui.callJS()
	 * from other WebViews. This way, the main WebView becomes a
	 * mediator, which is a good design because native access is
	 * restricted to one point.
	 */
	virtual void customEvent(const MAEvent& event);

	/**
	 * This method is called when a key is pressed.
	 * Forwards the event to PhoneGapMessageHandler.
	 * Override to implement your own behaviour.
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode);

	/**
	 * Run JavaScript code in the main WebView
	 * of this moblet.
	 * @param script JavaScript code to evaluate.
	 */
	virtual void callJS(const MAUtil::String& script);

	/**
	 * Evaluate JavaScript code in a WebView.
	 * @param webViewHandle The MoSync handle to the WebView in which
	 * to evaluate the script (this handle is an integer id).
	 * @param script JavaScript string.
	 */
	virtual void callJS(
		MAWidgetHandle webViewHandle,
		const MAUtil::String& script);


	/**
	 * Sends the Device Screen size to JavaScript.
	 */
	virtual void sendDeviceScreenSizeToJavaScript();

	/**
	 * Sends the web view handle to JavaScript.
	 */
	virtual void sendWebViewHandleToJavaScript();

	/**
	 * Enable JavaScript to C++ communication.
	 */
	virtual void enableWebViewMessages();

	/**
	 * Disable JavaScript to C++ communication.
	 */
	virtual void disableWebViewMessages();

	/**
	 * Extract HTML/CSS/JS/Media files to the local file system.
	 */
	virtual void extractFileSystem();

	/**
	 * @return true if the checksum has changed (or if the old
	 * value did not exist, such as on first time load).
	 */
	virtual bool checksumHasChanged();

	/**
	 * Write the current checksum to file.
	 */
	virtual void writeChecksum();

protected:

	/**
	 * The screen widget that is the root of the UI.
	 */
	NativeUI::Screen* mScreen;

	/**
	 * The WebView widget that displays the application UI.
	 */
	NativeUI::WebView* mWebView;

	/**
	 * JavaScript message listener.
	 */
	HybridMoblet_WebViewListener* mWebViewListener;

	/**
	 * File utility object.
	 */
	FileUtil* mFileUtil;

	/**
	 * Handles messages sent from JavaScript.
	 */
	MessageHandler* mMessageHandler;

	/**
	 * Is the moblet initialized?
	 */
	bool mInitialized;

}; // class

} // namespace

#endif

/*! @} */
