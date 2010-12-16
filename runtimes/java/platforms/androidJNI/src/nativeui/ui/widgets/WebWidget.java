package com.mosync.nativeui.ui.widgets;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.webkit.WebView;

/**
 * A WebWidget is graphical element that displays a webpage
 * on the screen.
 * 
 * @author fmattias
 */
public class WebWidget extends Widget
{
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
			throws PropertyConversionException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		WebView webView = (WebView) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_URL ) )
		{
			webView.loadUrl( value );
		}
		
		return true;
	}
}