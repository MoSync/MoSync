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
