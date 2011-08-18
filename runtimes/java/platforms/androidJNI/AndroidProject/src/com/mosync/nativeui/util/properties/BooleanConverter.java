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

/**
 * Class responsible for converting a boolean string that contains
 * 'true' or 'false' to a primitive boolean.
 * 
 * @author fmattias
 */
public class BooleanConverter
{
	/**
	 * Converts the given boolean string to a boolean value.
	 * 
	 * @param bool A string containing a boolean value, 'true' or 'false'.
	 * 
	 * @return The converted boolean.
	 * @throws PropertyConversionException if the boolean could not be converted.
	 */
	public static boolean convert(String bool) throws PropertyConversionException
	{
		if( bool == null )
		{
			throw new PropertyConversionException( "Input string is null" );
		}
		
		if( bool.equals( "true" ) || bool.equals( "1" ) )
		{
			return true;
		}
		else if( bool.equals( "false" ) || bool.equals( "0" ) )
		{
			return false;
		}
		else
		{
			throw new PropertyConversionException( "Bad boolean value: " + bool );
		}
	}
}
