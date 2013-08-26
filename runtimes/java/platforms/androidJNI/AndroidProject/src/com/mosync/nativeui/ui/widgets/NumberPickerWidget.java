/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
