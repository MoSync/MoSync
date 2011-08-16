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

package com.mosync.nativeui.ui.widgets;

import android.webkit.WebView;

import com.mosync.internal.generated.IX_WIDGET;
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