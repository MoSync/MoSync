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

import android.widget.TextView;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

public class LabelWidget extends Widget
{
	public LabelWidget(int handle, TextView view)
	{
		super(handle, view);
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		TextView textView = (TextView) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_TEXT ) )
		{
			textView.setText( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_FONT_COLOR ) )
		{
			textView.setTextColor( ColorConverter.convert( value ) );
		}
		else
		{
			return false;
		}
		
		return true;
	}
}
