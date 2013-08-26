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

import android.widget.CheckBox;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A checkbox widget, that can be toggled and sends events when
 * it is clicked.
 * 
 * @author fmattias
 */
public class CheckBoxWidget extends Widget
{
	/**
	 * Constructor.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param checkbox A checkbox wrapped by this widget.
	 */
	public CheckBoxWidget(int handle, CheckBox checkBox)
	{
		super( handle, checkBox );
		// TODO Auto-generated constructor stub
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		CheckBox checkBox = (CheckBox) getView( );
		if( property.equals( IX_WIDGET.MAW_CHECK_BOX_CHECKED ) )
		{
			boolean checked = BooleanConverter.convert( value );
			checkBox.setChecked( checked );
			
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
		if( property.equals( IX_WIDGET.MAW_CHECK_BOX_CHECKED ) )
		{
			CheckBox checkBox = (CheckBox) getView( );
			return Boolean.toString( checkBox.isChecked( ) );
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
