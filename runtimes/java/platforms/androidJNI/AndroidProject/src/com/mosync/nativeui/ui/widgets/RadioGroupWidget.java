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

import java.util.ArrayList;
import java.util.List;

import android.util.Log;
import android.widget.RadioGroup;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * a radio group widget, that acts like a container for radio buttons.
 * @author emma
 */
public class RadioGroupWidget extends Widget
{
	private List<RadioButtonWidget> mButtons = new ArrayList<RadioButtonWidget>( );

	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param radioGroup A radio group wrapped by this widget.
	 */
	public RadioGroupWidget(int handle, RadioGroup radioGroup)
	{
		super( handle, radioGroup );
	}

	/**
	 * Add a radio button view to this group.
	 * @param button The button.
	 */
	public void addButton(RadioButtonWidget button)
	{
		RadioGroup radioGroup = (RadioGroup) getView( );
//		button.setId(handle);
		radioGroup.addView(button.getView());
		mButtons.add(button);
	}

	/**
	 * Get the handle of a radio button widget.
	 * @param id The radio button id.
	 * @return The radio button widget handle.
	 */
	public int getButtonHandle(int id)
	{
		for (int i=0; i < mButtons.size(); i++)
		{
			if ( mButtons.get(i).getId() == id )
			{
				return mButtons.get(i).getHandle();
			}
		}
		return -1;
	}

	/**
	 * Check the button.
	 * @param id The radio button id.
	 */
	public void checkRadioButton(int id)
	{
		RadioGroup radioGroup = (RadioGroup) getView( );
		radioGroup.check(id);
	}

	/**
	 * Get checked radio button id.
	 * @return The radio button id.
	 */
	public int getChecked()
	{
		RadioGroup radioGroup = (RadioGroup) getView( );
		return radioGroup.getCheckedRadioButtonId();
	}

	@Override
	public int addChildAt(Widget child,int index)
	{
		if ( !(child instanceof RadioButtonWidget) )
		{
			Log.e("@@MoSync","maWidgetAddChild error: RadioGroupWidget can only add RadioButtons.");
			return IX_WIDGET.MAW_RES_INVALID_LAYOUT;
		}

		addButton((RadioButtonWidget) child);

		return IX_WIDGET.MAW_RES_OK;
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}

		if( property.equals( IX_WIDGET.MAW_RADIO_GROUP_CLEAR_CHECK ) )
		{
			RadioGroup radioGroup = (RadioGroup) getView( );
			radioGroup.clearCheck();
		}
		else if( property.equals( IX_WIDGET.MAW_RADIO_GROUP_SELECTED ) )
		{
			checkRadioButton(IntConverter.convert(value));
		}
		else if( property.equals( IX_WIDGET.MAW_RADIO_GROUP_ADD_VIEW ) )
		{
			IntConverter.convert(value);
			// Deprecated. Use maWidgetAddChild instead.
			return false;
		}
		else
		{
			return false;
		}
		return true;
	}

	@Override
	public String getProperty(String property)
	{
		if ( property.equals( IX_WIDGET.MAW_RADIO_GROUP_SELECTED ))
		{
			int checkedButtonHandle = getButtonHandle( getChecked() );
			if ( checkedButtonHandle != -1 )
				return Integer.toString(checkedButtonHandle);
		}

		return INVALID_PROPERTY_NAME;
	}

}