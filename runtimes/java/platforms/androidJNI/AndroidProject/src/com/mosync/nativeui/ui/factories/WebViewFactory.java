package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.content.Context;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.WebWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a web view which sends MoSync events for some of its
 * state changes.
 * 
 * @author Mikael Kindborg
 */
public class WebViewFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		// Create the Android WebView component.
		WebView webView = new MoSyncWebView(activity);
		
		// Java script is enabled by default on iOS so let make it so here too.
		// This might affect memory consumption / performance.
		webView.getSettings().setJavaScriptEnabled(true);
		
		// Create the widget proxy object that MoSync uses to manage
		// the WebView.
		WebWidget webWidget = new WebWidget(handle, webView); 

		// Create a WebViewClient object to handle things such as url detection.
		webView.setWebViewClient(new MoSyncWebViewClient(webWidget));
		
		return webWidget;
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
	 * @author Mikael Kindborg, Magnus Hult
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
		public boolean shouldOverrideUrlLoading(WebView view, String url)
		{
			String mosyncProtocol = "mosync://";
			
			if (url.startsWith(mosyncProtocol))
			{
				Log.i("@@@ MoSync", "Processing MoSync url: " + url);
				
				// Store the message in a data object.
				String messageString = url.substring(mosyncProtocol.length());
				int messageDataHandle = 
					MoSyncThread.getInstance().createDataObject(
						messageString.getBytes());
				
				// Post the MAW_EVENT_WEB_VIEW_URL_CHANGED message.
				EventQueue.getDefault().postWidgetEvent(
					IX_WIDGET.MAW_EVENT_CUSTOM_MESSAGE, 
					mWebWidget.getHandle(),
					messageDataHandle,
					0);
				
				// Abort further processing of this url by returning true.
				return true;
			}
			else
			{
				Log.i("@@@ MoSync", "Processing standard url: " + url);

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
	}
	// End of class MoSyncWebViewClient.
}
//End of class WebViewFactory.
