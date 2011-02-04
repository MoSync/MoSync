package com.mosync.nativeui.util.properties;

import java.util.HashMap;

import android.view.Gravity;

import com.mosync.nativeui.core.Types;

/**
 * Class responsible for converting a string to an android horizontal
 * alignment property. 
 * 
 * @author fmattias
 */
public class HorizontalAlignment
{
	/**
	 * Map from mosync string property to android int gravity.
	 */
	public static HashMap<String, Integer> m_mosyncHorizontal = new HashMap<String, Integer>( );
	
	static
	{	
		m_mosyncHorizontal.put( Types.WIDGET_HORIZONTAL_LEFT, Gravity.LEFT );
		m_mosyncHorizontal.put( Types.WIDGET_HORIZONTAL_CENTER, Gravity.CENTER_HORIZONTAL );
		m_mosyncHorizontal.put( Types.WIDGET_HORIZONTAL_RIGHT, Gravity.RIGHT );
	}
	
	/**
	 * Convert the given horizontal alignment property to a
	 * native android gravity.
	 * 
	 * @param value A mosync horizontal alignment property.
	 * @return the converted android gravity.
	 * @throws PropertyConversionException if the given property could not be converted.
	 */
	public static int convert(String value) throws PropertyConversionException
	{
		Integer androidGravity = m_mosyncHorizontal.get( value );
		
		if( androidGravity != null)
		{
			return androidGravity;
		}
		else
		{
			throw new PropertyConversionException( value );
		}
	}
	
	public static int clearHorizontalAlignment(int gravity)
	{
		return (gravity & ~( Gravity.LEFT | Gravity.CENTER_HORIZONTAL | Gravity.RIGHT ) );
	}
}
