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

import android.widget.Button;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * Wraps the behavior of a Button view. Some properties and event handling
 * are extensions of Widget.
 * 
 * @author fmattias
 */
public class ButtonWidget extends Widget
{
	
	public ButtonWidget(int handle, Button button)
	{
		super( handle, button );
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException
	{
		boolean propertySet = super.setProperty(property, value);
		if( propertySet )
		{
			return true;
		}
		
		Button button = (Button) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_TEXT ) )
		{
			button.setText( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_FONT_COLOR ) )
		{
			button.setTextColor( ColorConverter.convert( value ) );
		}
		else
		{
			return false;
		}
		
		return true;
	}

	
	
}
