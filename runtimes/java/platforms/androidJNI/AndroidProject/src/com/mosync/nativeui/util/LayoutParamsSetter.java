package com.mosync.nativeui.util;

import java.lang.reflect.Field;

import com.mosync.nativeui.ui.widgets.LayoutParams;

import android.view.ViewGroup;

/**
 * A class that allows meta layout params to be converted
 * between different layout params types.
 * 
 * @author fmattias
 */
public class LayoutParamsSetter
{
	/**
	 * Sets the layout params that can be set.
	 * 
	 * @param mosyncLayoutParams The MoSync layout params.
	 * @param nativeLayoutParams A layout params instance specific to a particular layout.
	 */
	public static void setPossibleParams(LayoutParams mosyncLayoutParams, ViewGroup.LayoutParams nativeLayoutParams)
	{
		if( hasField( nativeLayoutParams, "gravity" ) )
		{
			setGravity( mosyncLayoutParams, nativeLayoutParams );
		}
		
		if( hasField( nativeLayoutParams, "weight" ) )
		{
			setWeight( mosyncLayoutParams, nativeLayoutParams );
		}
		
		if( nativeLayoutParams instanceof ViewGroup.MarginLayoutParams )
		{
			setMargins( mosyncLayoutParams, (ViewGroup.MarginLayoutParams) nativeLayoutParams );
		}
	}
	
	/**
	 * Sets the gravity field on the given native layout params.
	 * 
	 * This function takes a ViewGroup.LayoutParams since there is not base class for
	 * LayoutParams that contain the gravity field.
	 * 
	 * @param mosyncLayoutParams The layout params whose horizontal and vertical alignment
	 *                           will be converted to a gravity setting.
	 * @param nativeLayoutParams The layout params whose gravity field will be updated.
	 */
	public static void setGravity(LayoutParams mosyncLayoutParams, ViewGroup.LayoutParams nativeLayoutParams)
	{
		try
		{
			Field gravityField = nativeLayoutParams.getClass( ).getField( "gravity" );
			int gravity = gravityField.getInt( nativeLayoutParams );
			
			boolean changed = false;
			if( mosyncLayoutParams.getHorizontalAlignment( ) != -1 )
			{
				gravity |= mosyncLayoutParams.getHorizontalAlignment( );
				changed = true;
			}
			if( mosyncLayoutParams.getHorizontalAlignment( ) != -1 )
			{
				gravity |= mosyncLayoutParams.getVerticalAlignment( );
				changed = true;
			}
			
			if( changed )
			{
				gravityField.setInt( nativeLayoutParams, gravity );
			}
		}
		catch(Exception e)
		{
			throw new IllegalArgumentException( "The nativeLayoutParams that you have passed does not have the 'gravity' field." );
		}
	}
	
	/**
	 * Sets the margin on the given layout from the mosync layout.
	 * 
	 * @param mosyncLayoutParams The MoSync layout params.
	 * @param nativeLayoutParams A layout params instance specific to a particular layout.
	 */
	public static void setMargins(LayoutParams mosyncLayoutParams, ViewGroup.MarginLayoutParams nativeLayoutParams)
	{
		nativeLayoutParams.leftMargin = mosyncLayoutParams.getMarginLeft( );
		nativeLayoutParams.topMargin = mosyncLayoutParams.getMarginTop( );
		nativeLayoutParams.rightMargin = mosyncLayoutParams.getMarginRight( );
		nativeLayoutParams.bottomMargin = mosyncLayoutParams.getMarginBottom( );
	}
	
	/**
	 * Sets the weight field on the given layout params from the value specified in the mosyncLayoutParams.
	 * 
	 * @param mosyncLayoutParams The layout params whose weight field will be converted to 
	 *                           a weight setting.
	 * @param nativeLayoutParams The layout params whose weight field will be updated.
	 */
	public static void setWeight(LayoutParams mosyncLayoutParams, ViewGroup.LayoutParams nativeLayoutParams)
	{
		try
		{
			Field weightField = nativeLayoutParams.getClass( ).getField( "weight" );
			weightField.setFloat( nativeLayoutParams, mosyncLayoutParams.weight );
	
		}
		catch(Exception e)
		{
			throw new IllegalArgumentException( "The nativeLayoutParams that you have passed does not have the 'weight' field." );
		}
	}
	
	/**
	 * Returns true if the given layout params has the specified field.
	 * 
	 * @param nativeLayoutParams A layout params instance specific to a particular layout.
	 * @param fieldName Name of a public field to check for.
	 * @return true if the given layout params has the given field, false otherwise.
	 */
	private static boolean hasField(ViewGroup.LayoutParams nativeLayoutParams, String fieldName)
	{
		try
		{
			nativeLayoutParams.getClass( ).getField( fieldName );
			return true;
		}
		catch(Exception e)
		{
			return false;
		}
	}
}
