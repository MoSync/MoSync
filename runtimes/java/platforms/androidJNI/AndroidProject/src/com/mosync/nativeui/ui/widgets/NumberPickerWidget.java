/* Copyright (C) 2012 MoSync AB

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

import android.widget.NumberPicker;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A Number Picker is a widget that enables the user to select a number
 * from a predefined range.
 *
 * @author emma tresanszki
 */
public class NumberPickerWidget extends Widget
{
	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param numberPicker A number picker wrapped by this widget.
	 */
	public NumberPickerWidget(int handle, NumberPicker numberPicker)
	{
		super( handle, numberPicker );
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}

		NumberPicker numberPicker = (NumberPicker) getView();

		if ( IntConverter.convert( value ) < 0 )
		{
			throw new InvalidPropertyValueException(property, value);
		}

		if( property.equals( IX_WIDGET.MAW_NUMBER_PICKER_VALUE ) )
		{
			numberPicker.setValue(IntConverter.convert(value));
		}
		else if( property.equals( IX_WIDGET.MAW_NUMBER_PICKER_MAX_VALUE ))
		{
			numberPicker.setMaxValue(IntConverter.convert(value));
		}
		else if( property.equals(IX_WIDGET.MAW_NUMBER_PICKER_MIN_VALUE ))
		{
			numberPicker.setMinValue(IntConverter.convert(value));
		}
		else
		{
			return false;
		}
		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		NumberPicker numberPicker = (NumberPicker) getView();

		if( property.equals( IX_WIDGET.MAW_NUMBER_PICKER_VALUE ) )
		{
			return Integer.toString(numberPicker.getValue());
		}
		else if( property.equals( IX_WIDGET.MAW_NUMBER_PICKER_MAX_VALUE ))
		{
			return Integer.toString( numberPicker.getMaxValue() );
		}
		else if( property.equals( IX_WIDGET.MAW_NUMBER_PICKER_MIN_VALUE ))
		{
			return Integer.toString( numberPicker.getMinValue() );
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
