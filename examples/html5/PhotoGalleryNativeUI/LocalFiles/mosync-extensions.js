/**
 * @file mosync-extensions.js
 * @author Mikael Kindborg
 *
 * This file contains custom extensions to JavaScript MoSync objects.
 */

/**
 * Constant to be used to reference the main WebView in an app.
 */
mosync.nativeui.MAIN_WEBVIEW = 0;

/**
 * Evaluate JavaScript code in another WebView. This provides a
 * way to pass messages and communicate between WebViews.
 *
 * @param webViewHandle The MoSync handle of the WebView widget.
 * Use mosync.nativeui.MAIN_WEBVIEW to refer to the main WebView
 * in the application (this is the hidden WebView in a JavaScript
 * NativeUI app).
 * @param js A string with JavaScript code.
 */
mosync.bridge.callJS = function(webViewHandle, js)
{
	mosync.bridge.send([
		"Custom",
		"CallJS",
		"" + webViewHandle,
		js]);
};

/**
 * Get the MoSync widget handle for the JavaScript NativeUI
 * element with the given ID.
 *
 * @param elementId A string id that identifies the widget (this
 * is the id of the DOM element that holds the widget info).
 */
mosync.nativeui.getWidgetHandleById = function(elementId)
{
	return mosync.nativeui.widgetIDList[elementId];
};
