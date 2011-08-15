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

package com.mosync.nativeui.util.properties;

import java.util.HashMap;

import android.view.Gravity;

import com.mosync.nativeui.core.Types;

/**
 * Class responsible for converting a string to an android vertical
 * alignment property. 
 * 
 * @author fmattias
 */
public class VerticalAlignment
{
	/**
	 * Map from mosync string property to android int gravity.
	 */
	public static HashMap<String, Integer> m_mosyncVertical = new HashMap<String, Integer>( );
	
	static
	{	
		m_mosyncVertical.put( Types.WIDGET_VERTICAL_TOP, Gravity.TOP );
		m_mosyncVertical.put( Types.WIDGET_VERTICAL_CENTER, Gravity.CENTER_VERTICAL );
		m_mosyncVertical.put( Types.WIDGET_VERTICAL_BOTTOM, Gravity.BOTTOM );
	}
	
	/**
	 * Convert the given vertical alignment property to a
	 * native android gravity.
	 * 
	 * @param value A mosync vertical alignment property.
	 * @return the converted android gravity.
	 * @throws PropertyConversionException if the given property could not be converted.
	 */
	public static int convert(String value) throws PropertyConversionException
	{
		Integer androidGravity = m_mosyncVertical.get( value );
		
		if( androidGravity != null)
		{
			return androidGravity;
		}
		else
		{
			throw new PropertyConversionException( value );
		}
	}
	
	public static int clearVerticalAlignment(int gravity)
	{
		return (gravity & ~( Gravity.TOP | Gravity.CENTER_VERTICAL | Gravity.BOTTOM ) );
	}
}
