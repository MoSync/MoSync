package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.WebWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a web view which sends mosync events for some of its
 * state changes.
 * 
 * @author Magnus hult
 */
public class WebViewFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		WebView webView = new WebView( activity );
		
		// Java script is enabled by default on iOS so let make it so here too.
		// This might affect memory consumption / performance.
		webView.getSettings( ).setJavaScriptEnabled( true );
		
		WebWidget webWidget = new WebWidget( handle, webView ); 
		webView.setWebViewClient( new UrlHookWebViewClient( webWidget ) );
		
		return webWidget;
	}
	
	/**
	 * An extended web client that hooks loading of web pages and
	 * sends an event.
	 * 
	 * @author Magnus Hult
	 */
	class UrlHookWebViewClient extends WebViewClient
	{
		/**
		 * Access to the wrapped web view, so that we can set the
		 * 'newurl' property.
		 */
		private WebWidget mWebWidget;

		/**
		 * @param webWidget The web view that the url overrider is bound to.
		 */
		public UrlHookWebViewClient(WebWidget webWidget)
		{
			mWebWidget = webWidget;
		}

		/**
		 * @see WebViewClient.shouldOverrideUrlLoading.
		 */
		public boolean shouldOverrideUrlLoading(WebView view, String url)
		{
			mWebWidget.setProperty( IX_WIDGET.MAW_WEB_VIEW_NEW_URL, url );
			EventQueue.getDefault( ).postWidgetEvent( IX_WIDGET.MAW_EVENT_WEB_VIEW_URL_CHANGED, mWebWidget.getHandle( ) );

			return super.shouldOverrideUrlLoading( view, url );
		}
	}
}
