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
}
