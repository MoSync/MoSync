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
