package com.mosync.nativeui.util.properties;

/**
 * Class responsible for converting a string to a float.
 * 
 * @author fmattias
 */
public class FloatConverter
{
	/**
	 * Converts an string to a integer.
	 * 
	 * @param value The integer string to convert.
	 * @return The integer string as an int.
	 * @throws PropertyConversionException if the string does not contain a valid integer.
	 */
	public static float convert(String value) throws PropertyConversionException
	{
		try
		{
			float convertedValue = Float.parseFloat( value );
			return convertedValue;
		}
		catch(NumberFormatException nfe)
		{
			throw new PropertyConversionException( value );
		}
	}
}
