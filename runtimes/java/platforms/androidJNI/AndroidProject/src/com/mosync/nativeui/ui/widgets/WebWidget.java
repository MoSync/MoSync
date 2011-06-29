package com.mosync.nativeui.ui.widgets;

import android.app.Activity;
import android.webkit.WebView;

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
	private String m_newUrl;

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
			// we use the content provider to load the file
			// from the application's local file system.
			String url = value;
			if (url.contains("://") || url.contains("javascript:"))
			{
				// There is a schema specified.
				webView.loadUrl(url);
			}
			else
			{
				// No schema present, add the content provider 
				// schema and authority and load the file.
				Activity activity = MoSyncThread.getInstance().getActivity();
				url = "content://" + activity.getPackageName() + "/" + url;
				webView.loadUrl(url);
			}
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_NEW_URL))
		{
			m_newUrl = value;
		}
		else if (property.equals("html"))
		{
			// Here we specify the base url of our content provider.
			// See class comment of this class for details:
			// com.mosync.java.android.MoSyncLocalFileContentProvider.java
			Activity activity = MoSyncThread.getInstance().getActivity();
			webView.loadDataWithBaseURL(
				"content://" + activity.getPackageName() + "/", 
				value, 
				"text/html", 
				"utf-8",
				null);
		}
		else if (property.equals("enableZoom"))
		{
			// Turn on or off zooming.
			boolean enable = BooleanConverter.convert(value);
			webView.getSettings().setBuiltInZoomControls(enable);
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
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_NEW_URL))
		{
			return m_newUrl;
		}
		else
		{
			return super.getProperty(property);
		}
	}
}