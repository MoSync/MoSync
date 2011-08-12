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
