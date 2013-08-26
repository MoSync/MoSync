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
 * Class responsible for converting a string to a long.
 *
 * @author emma
 */
public class LongConverter
{
	/**
	 * Converts an string to a long.
	 *
	 * @param value The long string to convert.
	 * @return The long string as an long.
	 * @throws PropertyConversionException if the string does not contain a valid long.
	 */
	public static long convert(String value) throws PropertyConversionException
	{
		try
		{
			long convertedValue = Long.parseLong(value);
			return convertedValue;
		}
		catch(NumberFormatException nfe)
		{
			throw new PropertyConversionException( value );
		}
	}
}
