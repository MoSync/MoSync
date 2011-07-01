package com.mosync.nativeui.ui.widgets;

import java.util.HashSet;

import android.app.Activity;
import android.util.Log;
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
	 * The pattern used when determining which urls
	 * will be "hooked" and sent MAW_EVENT_CUSTOM_MESSAGE
	 * Widget events.
	 */
	private String mUrlHookPattern;

	/**
	 * Collection of urls that should NOT be "hooked".
	 * This is used to make setting the url via the
	 * property "url" NOT being hooked. This way we
	 * prevent a "loop" in the hook mechanism.
	 * 
	 * TODO: Use a HashMap<String, Integer> and count 
	 * references is case same url is requested to load
	 * in parallel. A HashSet will fail in this case, 
	 * as it has just one occurrence of each url.
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
	 * Determine if the given url matches the hook pattern
	 * of thiw web view. If the length of the hook pattern
	 * if zero, then it is always a match.
	 * @param url The url to match.
	 * @return true if there is a match, false if not.
	 */
	public boolean wantsToHookUrl(String url)
	{
		if (mNonHookedUrls.contains(url))
		{
			// This url should NOT be hooked.
			// Remove it from the non-hooked urls.
			mNonHookedUrls.remove(url);
			return false;
		}
		else
		if (0 == mUrlHookPattern.length())
		{
			// When there is no hook pattern, the
			// url should NOT be hooked.
			return false;
		}
		else
		{
			// The reqexp match determines if the
			// url should be hooked.
			return url.matches(mUrlHookPattern);
		}
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
			
			if (url.contains("://") || url.contains("javascript:"))
			{
				// There is a schema specified.
				
				// Add this url to the non-hooked urls.
				mNonHookedUrls.add(url);
				
				// Load the url.
				webView.loadUrl(url);
			}
			else
			{
				// No schema present, add the local file:// schema
				// and load the file.
				
				// We need the activity.
				Activity activity = MoSyncThread.getInstance().getActivity();

				// We no longer use a content provider to load local files 
				// from the web view. Instead we use a file:// scheme for 
				// locally loaded pages, and for pages loaded over the
				// network we do not want to have this capability anyway.
				// Just want to keep line around for some time
				// as a reference in case it will be needed. Micke :)
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
			// TODO: MAW_WEB_VIEW_NEW_URL should be deprecated.
			m_newUrl = value;
		}
		else if (property.equals("html"))
		{
			// Here we specify the base url of our content provider.
			// See class comment of this class for details:
			// com.mosync.java.android.MoSyncLocalFileContentProvider.java
			Activity activity = MoSyncThread.getInstance().getActivity();
			webView.loadDataWithBaseURL(
				// Content provider is not used.
//				"content://" + activity.getPackageName() + "/", 
				// We use the path to the local application files
				// directory as the base url.
				"file://" + activity.getFilesDir().getAbsolutePath() + "/", 
				value, 
				"text/html", 
				"utf-8",
				null);
		}
		else if (property.equals("urlHookPattern"))
		{
			// Set the pattern used for url hooking.
			mUrlHookPattern = value;
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
		else if (property.equals("localFilesDirectory"))
		{
			Activity activity = MoSyncThread.getInstance().getActivity();
			String path = activity.getFilesDir().getAbsolutePath() + "/";
			Log.i("@@@ MoSync", "Property localFilesDirectory: " + path);
			return path;
		}
		else if (property.equals(IX_WIDGET.MAW_WEB_VIEW_NEW_URL))
		{
			// TODO: MAW_WEB_VIEW_NEW_URL should be deprecated.
			return m_newUrl;
		}
		else
		{
			return super.getProperty(property);
		}
	}
}