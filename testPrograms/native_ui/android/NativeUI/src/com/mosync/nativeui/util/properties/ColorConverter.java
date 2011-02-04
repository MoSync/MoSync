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
