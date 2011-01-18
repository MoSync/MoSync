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
		
		if( bool.equals( "true" ) )
		{
			return true;
		}
		else if( bool.equals( "false" ) )
		{
			return false;
		}
		else
		{
			throw new PropertyConversionException( "Bad boolean value: " + bool );
		}
	}
}
