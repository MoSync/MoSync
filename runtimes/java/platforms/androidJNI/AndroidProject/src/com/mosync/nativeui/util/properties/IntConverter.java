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

package com.mosync.nativeui.util.properties;

/**
 * A class responsible for converting a string to an interger.
 *
 * @author fmattias
 */
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

	/**
	 * Converts an string to a integer, if it's value is positive.
	 *
	 * @param value The integer string to convert.
	 * @return The integer string as an int.
	 * @throws PropertyConversionException if the string does not contain a valid integer, or
	 * a negative number.
	 */
	public static int convertPositiveNumber(String value) throws PropertyConversionException
	{
		try
		{
			int convertedValue = Integer.parseInt( value );
			if (convertedValue < 0)
			{
				throw new PropertyConversionException( value );
			}
			return convertedValue;
		}
		catch(NumberFormatException nfe)
		{
			throw new PropertyConversionException( value );
		}
	}
}
