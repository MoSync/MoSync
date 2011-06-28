package com.mosync.nativeui.ui.widgets;

import android.webkit.WebView;

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
		if( super.setProperty( property, value ) )
		{
			return true;
		}
		
		WebView webView = (WebView) getView( );
		if( property.equals( IX_WIDGET.MAW_WEB_VIEW_URL ) )
		{
			webView.loadUrl( value );
		}
		else if( property.equals( IX_WIDGET.MAW_WEB_VIEW_NEW_URL ) )
		{
			m_newUrl = value;
		}
		else if( property.equals( "html" ) )
		{
			webView.loadDataWithBaseURL(
				null, 
				value, 
				"text/html", 
				"utf-8",
				null);
		}
		else if( property.equals( "enableZoom" ) )
		{
			boolean enable = BooleanConverter.convert( value );
			webView.getSettings().setBuiltInZoomControls(enable);
		}
		
		return true;
	}
	
	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		WebView webView = (WebView) getView( );
		if( property.equals( IX_WIDGET.MAW_WEB_VIEW_URL ) )
		{
			return webView.getUrl( );
		}
		else if( property.equals( IX_WIDGET.MAW_WEB_VIEW_NEW_URL ) )
		{
			return m_newUrl;
		}
		else
		{
			return super.getProperty( property );
		}
	}
}