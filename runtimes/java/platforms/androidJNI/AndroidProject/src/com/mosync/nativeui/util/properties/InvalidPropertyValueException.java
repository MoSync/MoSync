package com.mosync.nativeui.util.properties;

/**
 * Exception that is thrown when the value is in the correct format
 * but out of range.
 * 
 * @author fmattias
 */
public class InvalidPropertyValueException extends RuntimeException
{
	private static final long serialVersionUID = 6820625518094170232L;
	
	public InvalidPropertyValueException(String property, String value)
	{
		super( "The property value: '" + value + 
			   "' is invalid for property '" + property + "'." );
	}
}
