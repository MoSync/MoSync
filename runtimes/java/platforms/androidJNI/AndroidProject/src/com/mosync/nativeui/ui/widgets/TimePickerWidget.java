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

import android.widget.TimePicker;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A view for selecting the time of day, in either 24 hour or AM/PM mode.
 * The hour, each minute digit, and AM/PM (if applicable) can be controlled by vertical spinners.
 * The hour can be entered by keyboard input.
 *
 * @author emma
 */
public class TimePickerWidget extends Widget
{
	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param progressbar A time picker wrapped by this widget.
	 */
	public TimePickerWidget(int handle, TimePicker timePicker)
	{
		super( handle, timePicker );
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

		TimePicker timePicker = (TimePicker) getView( );

		if( property.equals( IX_WIDGET.MAW_TIME_PICKER_CURRENT_HOUR ) )
		{
			int hour = IntConverter.convert(value);
			if ( hour < 0 || hour > 23 )
			{
				throw new InvalidPropertyValueException(property, value);
			}
			timePicker.setCurrentHour(IntConverter.convert( value ));
		}
		else if( property.equals( IX_WIDGET.MAW_TIME_PICKER_CURRENT_MINUTE ))
		{
			int minute = IntConverter.convert(value);
			if ( minute < 0 || minute > 59)
			{
				throw new InvalidPropertyValueException(property, value);
			}
			timePicker.setCurrentMinute(IntConverter.convert( value ));
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
		TimePicker timePicker = (TimePicker) getView( );

		if( property.equals( IX_WIDGET.MAW_TIME_PICKER_CURRENT_HOUR ) )
		{
			return Integer.toString( timePicker.getCurrentHour() );
		}
		else if( property.equals( IX_WIDGET.MAW_TIME_PICKER_CURRENT_MINUTE ) )
		{
			return Integer.toString( timePicker.getCurrentMinute() );
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
