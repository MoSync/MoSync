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

package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.content.Context;
import android.view.MotionEvent;
import android.view.View;
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
		WebView webView = new MoSyncWebView( activity );
		
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
	
	/**
	 * Custom WebView class.
	 * @author Mikael Kindborg
	 */
	static class MoSyncWebView extends WebView
	{
		public MoSyncWebView(Context context)
		{
			super(context);
			
			// Make the web view gaining focus on touch events.
			this.setOnTouchListener(new View.OnTouchListener() 
			{ 
				@Override
				public boolean onTouch(View view, MotionEvent event) 
				{
		           switch (event.getAction()) 
		           { 
		               case MotionEvent.ACTION_DOWN: 
		               case MotionEvent.ACTION_UP: 
		                   if (!view.hasFocus()) 
		                   { 
		                       view.requestFocus(); 
		                   }
		                   break; 
		           } 
		           return false; 
			    }
			});
		}

		/**
		 * TODO: Perhaps this is not needed.
		 * 
		 * This method is needed to make the WebView (this view) gain
		 * focus on touch events. See Android Issue: 7189
		 * http://code.google.com/p/android/issues/detail?id=7189
		 */
		@Override
		public boolean onCheckIsTextEditor() 
		{
			return true; 
		}
	}
}
