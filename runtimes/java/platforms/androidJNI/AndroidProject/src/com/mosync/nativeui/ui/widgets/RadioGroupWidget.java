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

import java.util.ArrayList;
import java.util.List;

import android.widget.RadioGroup;

import com.mosync.internal.generated.IX_WIDGET;
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
	 *
	 * @param id
	 * @return
	 */
	public RadioButtonWidget getButton(int id)
	{
		for (int i=0; i < mButtons.size(); i++)
		{
			if ( mButtons.get(i).getId() == id )
			{
				return mButtons.get(i);
			}
		}
		return null;
	}

	/**
	 *
	 * @param id
	 * @return
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

	public int getChecked()
	{
		RadioGroup radioGroup = (RadioGroup) getView( );
		return radioGroup.getCheckedRadioButtonId();
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
		else
		{
			return false;
		}
		return true;
	}

}
