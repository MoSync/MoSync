package com.mosync.nativeui.util.properties;

/**
 * Exception that is thrown when a system call was unsuccessful,
 * because it is not compatible with the current framework version.
 * 
 * @author emma tresanszki
 */
public class FeatureNotAvailableException extends RuntimeException
{
	private static final long serialVersionUID = 1L;
	
	public FeatureNotAvailableException(String feature)
	{
		super( "The system call: '" + feature +  
			   "' is not suported for this platform. '");
	}
}