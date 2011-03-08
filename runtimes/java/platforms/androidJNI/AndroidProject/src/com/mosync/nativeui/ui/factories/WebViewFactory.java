package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.WebWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.properties.PropertyConversionException;

public class WebViewFactory implements AbstractViewFactory {
	class ShouldOverrideUrlLoadingWebViewClient extends WebViewClient {
		private int mHandle;
		private WebWidget mWebWidget;

		public ShouldOverrideUrlLoadingWebViewClient(int handle, WebWidget webWidget) {
			mHandle = handle;
			mWebWidget = webWidget;
		}

		public boolean shouldOverrideUrlLoading(WebView view, String url) {
			try {
				mWebWidget.setProperty("newurl", url);
				EventQueue.getDefault( ).postWidgetEvent( IX_WIDGET.MAW_EVENT_WEB_VIEW_URL_CHANGED, mHandle );
			} catch (PropertyConversionException e) {
				// Ignore
			}
			return super.shouldOverrideUrlLoading(view, url);
		}
	}

	@Override
	public Widget create(Activity activity, int handle)
	{
		WebView webView = new WebView( activity );
		WebWidget webWidget = new WebWidget( handle, webView ); 
		webView.setWebViewClient( new ShouldOverrideUrlLoadingWebViewClient( handle, webWidget ) );
		return webWidget;
	}
}
