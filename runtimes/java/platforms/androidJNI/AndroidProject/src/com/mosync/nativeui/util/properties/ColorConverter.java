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

import android.graphics.Color;

/**
 * Class responsible for converting a string to an android color.
 * 
 * @author fmattias
 */
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
