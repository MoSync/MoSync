package com.mosync.nativeui.ui.widgets;

import java.util.HashSet;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
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
	 */
	private String m_newUrl;

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
			Log.i("@@@ MoSync", "Non-hooked url detected: " + url);

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

		Log.i("@@@ MoSync", "No hook detected for url: " + url);

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
			// Here we check if the url has schema specifier.
			// This is done so that is there is no schema
			// we use the file:// schema to load the file
			// from the application's local file system.
			String url = value;

			// Add this url to the non-hooked urls. We want urls
			// loaded by this property to bypass the hook mechanism,
			// to avoid "loops" in the loading process (can happen for
			// hard hooks).
			mNonHookedUrls.add(url);

			if (url.contains("://") || url.contains("javascript:"))
			{
				// There is a schema specified.

				// Load the url.
				webView.loadUrl(url);
			}
			else
			{
				// No schema present, add the local file:// schema
				// and load the file.

				// We need the activity.
				Activity activity = MoSyncThread.getInstance().getActivity();

//				// Old code kept as a reference.
//				// We no longer use a content provider to load local files
//				// from the web view. Instead we use a file:// scheme for
//				// locally loaded pages, and for pages loaded over the
//				// network we do not want to have this capability anyway.
//				// Just want to keep line around for some time
//				// as a reference in case it will be needed. /Micke
//				url = "content://" + activity.getPackageName() + "/" + url;

				// This is the way that use the file:// schema.
				url =
					"file://"
					+ activity.getFilesDir().getAbsolutePath()
					+ "/"
					+ url;

				// Load the url.
				webView.loadUrl(url);
			}
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_NEW_URL))
		{
			// TODO:  To be removed. MAW_WEB_VIEW_NEW_URL is deprecated.
			m_newUrl = value;
		}
		else if (property.equals("html"))
		{
			Activity activity = MoSyncThread.getInstance().getActivity();
			webView.loadDataWithBaseURL(
				// We use the path to the local application files
				// directory as the base url.
				// TODO: Update this path to the sub folder we will use.
				"file://" + activity.getFilesDir().getAbsolutePath() + "/",
				value,
				"text/html",
				"utf-8",
				null);

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
		}
		else if (property.equals("softHookPattern"))
		{
			Log.i("@@@ Mosync", "Setting softHookPattern to: " + value);

			// Set the pattern used for url hooking.
			mSoftHookPattern = value;
		}
		else if (property.equals("hardHookPattern"))
		{
			Log.i("@@@ Mosync", "Setting hardHookPattern to: " + value);

			// Set the pattern used for url hooking.
			mHardHookPattern = value;
		}
		else if (property.equals("enableZoom"))
		{
			// Turn on or off zooming.
			boolean enable = BooleanConverter.convert(value);
			webView.getSettings().setBuiltInZoomControls(enable);
		}
		else if (property.equals("navigate"))
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
		else if (property.equals("localFilesDirectory"))
		{
			// TODO: Update this path to the sub folder we will use
			// for MoSync "asset" files.
			Activity activity = MoSyncThread.getInstance().getActivity();
			String path = activity.getFilesDir().getAbsolutePath() + "/";
			Log.i("@@@ MoSync", "Property localFilesDirectory: " + path);
			return path;
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_NEW_URL))
		{
			// TODO: To be removed. MAW_WEB_VIEW_NEW_URL is deprecated.
			return m_newUrl;
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

			// Should we soft hook this url?
			int hookType = mWebWidget.checkHookType(url);
			if (IX_WIDGET.MAW_CONSTANT_SOFT == hookType ||
				IX_WIDGET.MAW_CONSTANT_HARD == hookType)
			{
				Log.i("@@@ MoSync", "Url is hooked: " + url);

				// Store the message in a data object.
				String messageString = url;
				int urlData = MoSyncThread.getInstance().createDataObject(
					messageString.getBytes());

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
			else
			{
				Log.i("@@@ MoSync", "Processing standard url: " + url);

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
				IX_WIDGET.MAW_CONSTANT_FAILED,
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
				"JsAlert: " + message,
				Toast.LENGTH_SHORT).show();

			// This is needed to end the alert state.
			result.confirm();

			return true;
		}
	}
}