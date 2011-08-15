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

public class IntConverter
{
	/**
	 * Converts an string to a integer.
	 * 
	 * @param value The integer string to convert.
	 * @return The integer string as an int.
	 * @throws PropertyConversionException if the string does not contain a valid integer.
	 */
	public static int convert(String value) throws PropertyConversionException
	{
		try
		{
			int convertedValue = Integer.parseInt( value );
			return convertedValue;
		}
		catch(NumberFormatException nfe)
		{
			throw new PropertyConversionException( value );
		}
	}
}
