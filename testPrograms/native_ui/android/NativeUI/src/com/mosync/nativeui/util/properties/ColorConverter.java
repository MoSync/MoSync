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

import android.graphics.Color;

public class ColorConverter
{

	/**
	 * Converts the given color string to a native android color.
	 * 
	 * @param color The color string, should be on the format "#RRGGBB", 
	 *              "0xRRGGBB" or "RRGGBB".
	 * 
	 * @return The converted color
	 * @throws PropertyConversionException if the color could not be converted.
	 */
	public static int convert(String color) throws PropertyConversionException
	{
		String nativeColor = color;
		if( color.startsWith( "0x" ) )
		{
			nativeColor = color.replaceFirst( "0x", "#" );
		}
		else if( !color.startsWith( "#" ) )
		{
			nativeColor = "#" + color;
		}
		
		try
		{
			return Color.parseColor( nativeColor );
		}
		catch(IllegalArgumentException iae)
		{
			throw new PropertyConversionException( color );
		}
	}
}
