package com.mosync.nativeui.util.properties;

/**
 * Exception that is thrown when a property conversion was unsuccessful.
 * 
 * @author fmattias
 */
public class PropertyConversionException extends RuntimeException
{
	private static final long serialVersionUID = 7497514582595613978L;
	
	public PropertyConversionException(String value)
	{
		super( "Could not convert: '" + value + "'." );
	}
}