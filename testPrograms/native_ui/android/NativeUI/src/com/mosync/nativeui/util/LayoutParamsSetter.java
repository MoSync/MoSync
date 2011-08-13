/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

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
			gravityField.setInt( nativeLayoutParams, mosyncLayoutParams.getHorizontalAlignment() | mosyncLayoutParams.getVerticalAlignment() );
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
