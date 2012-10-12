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
	virtual void openWormhole(
		Wormhole::HybridMoblet* moblet);

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
		NativeUI::WebView* webView,
		MAHandle data);

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

public: // On purpose!

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
	MessageHandler mMessageHandler;

	/**
	 * Is the moblet initialized?
	 */
	bool mInitialized;

}; // class

} // namespace

#endif

/*! @} */
