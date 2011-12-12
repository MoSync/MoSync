/* Copyright (C) 2011 MoSync AB

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

package com.mosync.nativeui.ui.widgets;

import java.util.HashSet;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.util.Log;
import android.view.MotionEvent;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A WebWidget is graphical element that displays a webpage
 * on the screen.
 *
 * @author fmattias
 */
public class WebWidget extends Widget
{
	/**
	 * TODO: Belongs to deprecated API. Remove when
	 * the deprecated API is removed.
	 * @deprecated
	 */
	@Deprecated
	private String m_newUrl;

	/**
	 * The base URL used when setting the URL and HTML
	 * properties.
	 */
	private String mBaseURL;

	/**
	 * The pattern used when determining which urls
	 * will be "soft hooked".
	 */
	private String mSoftHookPattern;

	/**
	 * The pattern used when determining which urls
	 * will be "hard hooked".
	 */
	private String mHardHookPattern;

	/**
	 * Collection of urls that should NOT be "hooked".
	 *
	 * This is used to make loading a url via the
	 * property "url" NOT being hooked. This way we
	 * prevent a "loop" in the hook mechanism.
	 */
	private HashSet<String> mNonHookedUrls = new HashSet<String>();

	/**
	 * Constructor
	 *
	 * Note: The id of the view is always equal to its handle.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param view A view wrapped by this widget.
	 */
	public WebWidget(int handle, WebView view)
	{
		super(handle, view);

		// This base URL is used for the URL and HTML properties.
		Activity activity = MoSyncThread.getInstance().getActivity();
		mBaseURL = "file://" + activity.getFilesDir().getAbsolutePath() + "/";
	}

	/**
	 * Factory method that creates an instance of this class.
	 * @param activity The Android activity.
	 * @param handle Widget handle (MoSync handle).
	 * @return A configured WebWidget.
	 */
	public static Widget create(Activity activity, int handle)
	{
		// Create the Android WebView component.
		WebView webView = new WebWidget.MoSyncWebView(activity);

		// Create the widget proxy object that MoSync uses to manage
		// the WebView.
		WebWidget webWidget = new WebWidget(handle, webView);

		// Create a WebViewClient object to handle things such as url detection.
		webView.setWebViewClient(new WebWidget.MoSyncWebViewClient(webWidget));

		// Create a WebChromeClient object.
		webView.setWebChromeClient(new WebWidget.MoSyncWebChromeClient());

		//Enable GeoLocation for webbased apps
		webView.getSettings().setGeolocationEnabled(true);

		return webWidget;
	}

	/**
	 * Determine if the given url matches a hook pattern
	 * set in this web view.
	 *
	 * Hard hooks take precedence over soft hooks in case both
	 * the sofy and hard hook pattern would match the url.
	 *
	 * Call this method only once per request (since we remove
	 * the url from the collection of hooked urls here).
	 *
	 * @param url The url to match.
	 * @return The hook type constant if there is a match, -1 if not.
	 */
	public int checkHookType(String url)
	{
		// Should we hook this url?
		if (mNonHookedUrls.contains(url))
		{
			//Log.i("@@@ MoSync", "Non-hooked url detected: " + url);

			// This url should NOT be hooked. Since the check
			// is now done, we remove it from the non-hooked urls.
			mNonHookedUrls.remove(url);
			return -1;
		}

		// Is there a hard hook?
		if (null != mHardHookPattern && 0 < mHardHookPattern.length())
		{
			// The reqexp match determines if the
			// url should be hooked.
			if (url.matches(mHardHookPattern))
			{
				Log.i("@@@ MoSync", "Hard hook detected: " + mHardHookPattern);
				return IX_WIDGET.MAW_CONSTANT_HARD;
			}
		}

		// Is there a soft hook?
		if (null != mSoftHookPattern && 0 < mSoftHookPattern.length())
		{
			// The reqexp match determines if the
			// url should be hooked.
			if (url.matches(mSoftHookPattern))
			{
				Log.i("@@@ MoSync", "Soft hook detected: " + mSoftHookPattern);
				return IX_WIDGET.MAW_CONSTANT_SOFT;
			}
		}

		//Log.i("@@@ MoSync", "No hook detected for url: " + url);

		// When there is no hook pattern, the url
		// should NOT be hooked.
		return -1;
	}


	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if (super.setProperty(property, value))
		{
			return true;
		}

		// Get the native Android WebView instance.
		WebView webView = (WebView) getView();

		if (property.equals(IX_WIDGET.MAW_WEB_VIEW_URL))
		{
			String url = value;

			// Add this url to the non-hooked urls. We want urls
			// loaded by this property to bypass the hook mechanism,
			// to avoid "loops" in the loading process (can happen for
			// hard hooks).
			mNonHookedUrls.add(url);

			// Here we check if the url has schema specifier.
			// This is done so that is there is no schema
			// we use the file:// schema to load the file
			// from the application's local file system.
			if (url.contains("://") || url.contains("javascript:"))
			{
				// There is a schema specified.
				// Load the url.
				webView.loadUrl(url);
			}
			else
			{
//				// Old code kept as a reference.
//				// We no longer use a content provider to load local files
//				// from the web view. Instead we use a file:// scheme for
//				// locally loaded pages, and for pages loaded over the
//				// network we do not want to have this capability anyway.
//				// Just want to keep line around for some time
//				// as a reference in case it will be needed. /Micke
//				url = "content://" + activity.getPackageName() + "/" + url;

				// No schema present
				// Add the local file:// schema and load the url.
				url = mBaseURL + url;
				webView.loadUrl(url);
			}
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_NEW_URL))
		{
			// TODO:  To be removed. MAW_WEB_VIEW_NEW_URL is deprecated.
			m_newUrl = value;
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_HTML))
		{
//			// Old code kept as a reference.
//			// Here we specify the base url of our content provider.
//			// See class comment of this class for details:
//			// com.mosync.java.android.MoSyncLocalFileContentProvider.java
//			Activity activity = MoSyncThread.getInstance().getActivity();
//			webView.loadDataWithBaseURL(
//				"content://" + activity.getPackageName() + "/",
//				value,
//				"text/html",
//				"utf-8",
//				null);

			// Load the HTML data using the currently set base url.
			webView.loadDataWithBaseURL(
				mBaseURL,
				value, // HTML data.
				"text/html",
				"utf-8",
				null);

		}
		else if (property.equals("baseUrl")) //IX_WIDGET.MAW_WEB_VIEW_BASE_URL))
		{
			mBaseURL = value;
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_SOFT_HOOK))
		{
			Log.i("@@@ Mosync", "Setting softHookPattern to: " + value);

			// Set the pattern used for url hooking.
			mSoftHookPattern = value;
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_HARD_HOOK))
		{
			Log.i("@@@ Mosync", "Setting hardHookPattern to: " + value);

			// Set the pattern used for url hooking.
			mHardHookPattern = value;
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_ENABLE_ZOOM))
		{
			// Turn on or off zooming.
			boolean enable = BooleanConverter.convert(value);
			webView.getSettings().setBuiltInZoomControls(enable);
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_NAVIGATE))
		{
			if ("back".equals(value))
			{
				if (webView.canGoBack())
				{
					webView.goBack();
				}
			}
			else if ("forward".equals(value))
			{
				if (webView.canGoForward())
				{
					webView.goForward();
				}
			}
		}
		else if (property.equals("androidAddJavaScriptInterfaceForActivity"))
		{
			// Make the string in value a global JavaScript variable
			// that refers to the application's activity.
			Activity activity = MoSyncThread.getInstance().getActivity();
			webView.addJavascriptInterface(activity, value);
		}
		else if (property.equals("androidAddJavaScriptInterfaceForWebView"))
		{
			// Make the string in value a global JavaScript variable
			// that refers to this WebView instance.
			webView.addJavascriptInterface(webView, value);
		}

		else if( property.equals( IX_WIDGET.MAW_WEB_VIEW_HORIZONTAL_SCROLL_BAR_ENABLED ) )
		{
			webView.setHorizontalScrollBarEnabled(BooleanConverter.convert(value));
		}
		else if ( property.equals(IX_WIDGET.MAW_WEB_VIEW_VERTICAL_SCROLL_BAR_ENABLED ))
		{
			webView.setVerticalScrollBarEnabled(BooleanConverter.convert(value));
		}

		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		WebView webView = (WebView) getView();
		if (property.equals(IX_WIDGET.MAW_WEB_VIEW_URL))
		{
			return webView.getUrl();
		}
		else if (property.equals("baseUrl")) //IX_WIDGET.MAW_WEB_VIEW_BASE_URL))
		{
			return mBaseURL;
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_NEW_URL))
		{
			// TODO: To be removed. MAW_WEB_VIEW_NEW_URL is deprecated.
			return m_newUrl;
		}
		else if( property.equals( IX_WIDGET.MAW_WEB_VIEW_HORIZONTAL_SCROLL_BAR_ENABLED ))
		{
			return Boolean.toString(webView.isHorizontalScrollBarEnabled());
		}
		else if( property.equals( IX_WIDGET.MAW_WEB_VIEW_VERTICAL_SCROLL_BAR_ENABLED ))
		{
			return Boolean.toString(webView.isVerticalScrollBarEnabled());
		}
		else if (property.equals( IX_WIDGET.MAW_WEB_VIEW_NAVIGATE ))
		{
			String status = "";
			if (webView.canGoBack())
			{
				status = status + "back";
			}
			if (webView.canGoForward())
			{
				status = status + "forward";
			}
			return status;
		}
		else
		{
			return super.getProperty(property);
		}
	}

	/**
	 * Custom WebView class.
	 *
	 * @author Mikael Kindborg
	 */
	static class MoSyncWebView extends WebView
	{
		public MoSyncWebView(Context context)
		{
			super(context);

			// Java script is enabled by default on iOS so let make
			// it so here too.
			// This might affect memory consumption / performance.
			this.getSettings().setJavaScriptEnabled(true);

			this.setVerticalScrollbarOverlay(true);
		}

		/**
		 * Make the web view gaining focus on touch events.
		 * See Android Issue: 7189
		 * http://code.google.com/p/android/issues/detail?id=7189
		 */
		@Override
		public boolean onTouchEvent(MotionEvent event)
		{
			// On touch down and touch up events we give
			// focus to the web view.
			if (event.getAction() == MotionEvent.ACTION_DOWN ||
				event.getAction() == MotionEvent.ACTION_UP)
			{
				if (!this.hasFocus())
				{
					this.requestFocus();
				}
			}

			// Continue with default event processing.
			return super.onTouchEvent(event);
		}

		// Note: Some advise on the web may say that onCheckIsTextEditor
		// needs to be implemented to fix Android Issue: 7189
		// For example this issue:
		// http://code.google.com/p/android/issues/detail?id=7189
		// But this solution causes a crash on HTC Flyer.
		// Do NOT override onCheckIsTextEditor for now!
	}
	// End of class MoSyncWebView.

	/**
	 * An extended web client that hooks loading of web pages and
	 * sends an event.
	 *
	 * @author Mikael Kindborg
	 */
	static class MoSyncWebViewClient extends WebViewClient
	{
		/**
		 * Access to the wrapped web view, so that we can set the
		 * 'newurl' property.
		 */
		private WebWidget mWebWidget;

		/**
		 * @param webWidget The web view that the url overrider is bound to.
		 */
		public MoSyncWebViewClient(WebWidget webWidget)
		{
			mWebWidget = webWidget;
		}

		/**
		 * Called when a url is to be resolved.
		 *
		 * Here we inspect the url that is being loaded. If the url is
		 * a mosync:// url, we post an MAW_EVENT_CUSTOM_MESSAGE event
		 * to the MoSync event queue. For all other urls we post an
		 * MAW_EVENT_WEB_VIEW_URL_CHANGED event.
		 *
		 * TODO: Note that the storage mechanism used by
		 * MAW_EVENT_WEB_VIEW_URL_CHANGED may not work if multiple
		 * urls are loaded quickly. As soon as the next url is loaded
		 * the property "newurl" (MAW_WEB_VIEW_NEW_URL) is overwritten.
		 * This may need to be redesigned.
		 *
		 * Note that MAW_EVENT_CUSTOM_MESSAGE uses a lookup table for
		 * messages, so that it works if multiple messages are present
		 * in the event queue.
		 */
		@Override
		public boolean shouldOverrideUrlLoading(WebView view, String url)
		{
			Log.i("@@@ MoSync",
				"MoSyncWebViewClient.shouldOverrideUrlLoading url: " + url);

			// Should we hook this url?
			int hookType = mWebWidget.checkHookType(url);
			if (IX_WIDGET.MAW_CONSTANT_SOFT == hookType ||
				IX_WIDGET.MAW_CONSTANT_HARD == hookType)
			{
				//Log.i("@@@ MoSync", "Url is hooked: " + url);

				// Store the message in a data object.
				String messageString = url;
				int urlData = MoSyncThread.getInstance().createDataObject(
					0, // Zero makes the system create a new placeholder.
					messageString.getBytes() // Data content.
					);

				// Post message.
				EventQueue.getDefault().postWidgetEvent(
					IX_WIDGET.MAW_EVENT_WEB_VIEW_HOOK_INVOKED,
					mWebWidget.getHandle(),
					hookType,
					urlData);

				if (IX_WIDGET.MAW_CONSTANT_HARD == hookType)
				{
					// Abort further processing of this url by returning true.
					return true;
				}
				else
				{
					return false;
				}
			}
			//Here we have two default URLs that should always be processed outside.
			//if the user wants to override them it is possible since user HOOKs
			//have priority
			else if (url.startsWith("rtsp:"))
			{
				//Start a media view intent for the video links
				Uri uri = Uri.parse(url);
			    Intent intent = new Intent(Intent.ACTION_VIEW, uri);
			    mWebWidget.getView().getContext().startActivity(intent);
			    return true;
			}
			else if(url.startsWith("tel:"))
			{
				//by default we should open the PhoneApp when this URL
				//is loaded
				Uri uri = Uri.parse(url);
			    Intent intent = new Intent(Intent.ACTION_DIAL, uri);
			    mWebWidget.getView().getContext().startActivity(intent);
			    return true;
			}
			else
			{
				//Log.i("@@@ MoSync", "Processing standard url: " + url);

				// TODO: Remove the following code for handling
				// the deprecated MAW_EVENT_WEB_VIEW_URL_CHANGED.

				// Set the "newurl" property of the web view widget.
				// Note that this will overwrite the previous url even
				// if the corresponding MAW_EVENT_WEB_VIEW_URL_CHANGED
				// has not been processed.
				mWebWidget.setProperty(IX_WIDGET.MAW_WEB_VIEW_NEW_URL, url);

				// Post the MAW_EVENT_WEB_VIEW_URL_CHANGED message.
				EventQueue.getDefault().postWidgetEvent(
					IX_WIDGET.MAW_EVENT_WEB_VIEW_URL_CHANGED,
					mWebWidget.getHandle());

				// Do default url processing of the url.
				return false;
			}
		}

		/**
		 * Called when a page starts loading.
		 */
		@Override
		public void onPageStarted(WebView view, String url, Bitmap favIcon)
		{
			Log.i("@@@ MoSync",
				"MoSyncWebViewClient.onPageStarted url: " + url);

			EventQueue.getDefault().postWidgetEvent(
				IX_WIDGET.MAW_EVENT_WEB_VIEW_CONTENT_LOADING,
				mWebWidget.getHandle(),
				IX_WIDGET.MAW_CONSTANT_STARTED,
				0);
		}

		/**
		 * Called when a page has finished loading.
		 */
		@Override
		public void onPageFinished(WebView view, String url)
		{
			Log.i("@@@ MoSync",
				"MoSyncWebViewClient.onPageFinished url: " + url);

			EventQueue.getDefault().postWidgetEvent(
				IX_WIDGET.MAW_EVENT_WEB_VIEW_CONTENT_LOADING,
				mWebWidget.getHandle(),
				IX_WIDGET.MAW_CONSTANT_DONE,
				0);
		}

		/**
		 * Called when an error occurs.
		 */
		@Override
		public void onReceivedError(
			WebView view,
			int errorCode,
			String description,
			String failingUrl)
		{
			Log.i("@@@ MoSync",
				"MoSyncWebViewClient.onReceivedError url: " + failingUrl
				+ " error: " + description);

			EventQueue.getDefault().postWidgetEvent(
				IX_WIDGET.MAW_EVENT_WEB_VIEW_CONTENT_LOADING,
				mWebWidget.getHandle(),
				IX_WIDGET.MAW_CONSTANT_ERROR,
				0);
		}
	}
	// End of class MoSyncWebViewClient.

	static class MoSyncWebChromeClient extends WebChromeClient
	{
		/**
		 * Report a JavaScript error message to the host application.
		 *
		 * TODO: Move to a separate file and load conditionally, like the
		 * Bluetooth class, since this method is available from API level 7.
		 */
		@Override
		public void onConsoleMessage(String message, int lineNumber, String sourceID)
		{
			Log.i("@@@ MoSync",
				"MoSyncWebChromeClient.onConsoleMessage: " + message);
		}

		/**
		 * Tell the client to display a JavaScript alert dialog.
		 */
		@Override
		public boolean onJsAlert(
			WebView view,
			String url,
			String message,
			JsResult result)
		{
			Log.i("@@@ MoSync",
				"MoSyncWebChromeClient.onJsAlert: " + message);

			Toast.makeText(
				view.getContext(),
				message,
				Toast.LENGTH_LONG).show();

			// This is needed to end the alert state.
			result.confirm();

			return true;
		}
	}
}
