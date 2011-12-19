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

import android.widget.ToggleButton;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A toggle button widget, that displays checked/unchecked states as a button with a "light" indicator and
 * by default accompanied with the text "ON" or "OFF".
 * Sends events when it is clicked.
 *
 * @author emma tresanszki
 */
public class ToggleButtonWidget extends Widget
{
	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param toggleBtn A toggle button wrapped by this widget.
	 */
	public ToggleButtonWidget(int handle, ToggleButton toggleBtn)
	{
		super( handle, toggleBtn );
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}

		ToggleButton toggleBtn = (ToggleButton) getView( );
		if( property.equals( IX_WIDGET.MAW_TOGGLE_BUTTON_CHECKED ) )
		{
			boolean checked = BooleanConverter.convert( value );
			toggleBtn.setChecked( checked );

			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		if( property.equals( IX_WIDGET.MAW_TOGGLE_BUTTON_CHECKED ) )
		{
			ToggleButton toggleBtn = (ToggleButton) getView( );
			return Boolean.toString( toggleBtn.isChecked( ) );
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
