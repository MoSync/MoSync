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

import android.widget.RadioButton;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A radio button widget is a two-states button that can be either checked
 * or unchecked.
 * Radio buttons are automatically grouped inside RadioGroup element
 * so that no more than one can be selected at a time.
 * @author emma
 */
public class RadioButtonWidget extends Widget
{
	private int mHandle = -1;

	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param radioButton A radioButton wrapped by this widget.
	 */
	public RadioButtonWidget(int handle, RadioButton radioButton)
	{
		super( handle, radioButton );
		mHandle = handle;
	}

	public int getHandle()
	{
		return mHandle;
	}

	/**
	 * Get the id of this radio button.
	 * @return
	 */
	public int getId()
	{
		RadioButton radioButton = (RadioButton) getView( );
		return radioButton.getId();
	}

	public void setId(int id)
	{
		RadioButton radioButton = (RadioButton) getView( );
		radioButton.setId(id);
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}

		RadioButton radioButton = (RadioButton) getView( );
		if( property.equals( IX_WIDGET.MAW_RADIO_BUTTON_TOGGLE ) )
		{
			radioButton.toggle();
		}
		else if( property.equals( IX_WIDGET.MAW_RADIO_BUTTON_TEXT ) )
		{
			radioButton.setText(value);
		}
		else if( property.equals( IX_WIDGET.MAW_RADIO_BUTTON_TEXT_COLOR ) )
		{
			radioButton.setTextColor( ColorConverter.convert( value ) );
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
		RadioButton radioButton = (RadioButton) getView( );
		if( property.equals( IX_WIDGET.MAW_RADIO_BUTTON_TEXT ) )
		{
			return radioButton.getText().toString();
		}
		else if( property.equals( IX_WIDGET.MAW_RADIO_BUTTON_CHECKED ) )
		{
			return Boolean.toString( radioButton.isChecked() );
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
