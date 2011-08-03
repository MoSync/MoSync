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
}