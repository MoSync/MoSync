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

import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A class that represents a standardized list element, that
 * contains a label and an optional icon.
 * 
 * @author fmattias
 */
public class ListItemWidget extends Layout
{
	/**
	 * Label in the list layout.
	 */
	private TextView m_label = null;
	
	/**
	 * Icon in the list layout.
	 */
	private ImageView m_icon = null;

	/**
	 * 
	 * @param handle
	 * @param layout
	 * @param label
	 * @param icon
	 */
	public ListItemWidget(int handle, ViewGroup layout, TextView label, ImageView icon)
	{
		super( handle, layout );
		
		m_label = label;
		m_icon = icon;
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( property.equals( IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT ) )
		{
			m_label.setText( value );
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_ITEM_ICON ) )
		{
			int imageHandle = IntConverter.convert( value );
			m_icon.setImageBitmap( NativeUI.getBitmap( imageHandle ) );
		}
		else
		{
			return super.setProperty( property, value );
		}
		
		return true;
	}
	
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new RelativeLayout.LayoutParams( mosyncLayoutParams.getWidth( ), mosyncLayoutParams.getHeight( ) );
	}
}