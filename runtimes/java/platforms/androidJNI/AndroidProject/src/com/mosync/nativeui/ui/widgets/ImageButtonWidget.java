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

package com.mosync.nativeui.ui.widgets;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.widget.ImageButton;
import android.widget.ImageView;

/**
 * Wraps the behavior of a ImageButton view. Some properties and event handling
 * are extensions of Widget.
 * 
 * @author fmattias
 */
public class ImageButtonWidget extends Widget
{
	/**
	 * Constructor.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param button
	 *            An image button wrapped by this widget.
	 */
	public ImageButtonWidget(int handle, ImageButton imageButton)
	{
		super( handle, imageButton );
		
		/**
		 * Remove the background drawable (we don't want the default button background.).
		 */
		imageButton.setBackgroundDrawable( null );
		
		/**
		 * Defaults to no scaling.
		 */
		imageButton.setScaleType( ImageView.ScaleType.CENTER ); 
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( property
				.equals( IX_WIDGET.MAW_IMAGE_BUTTON_BACKGROUND_IMAGE ) )
		{
			int imageHandle = IntConverter.convert( value );
			Bitmap background = NativeUI.getBitmap( imageHandle );
			if( background != null )
			{
				getView( ).setBackgroundDrawable(
						new BitmapDrawable( background ) );
			}
		}
		else if( property
				.equals( IX_WIDGET.MAW_IMAGE_BUTTON_IMAGE ) )
		{
			int imageHandle = IntConverter.convert( value );
			Bitmap foreground = NativeUI.getBitmap( imageHandle );
			if( foreground != null )
			{
				ImageButton imageButton = (ImageButton) getView( );
				imageButton.setImageBitmap( foreground );
				imageButton.setScaleType( ImageView.ScaleType.CENTER );	
			}
		}		
		/*
		// this doesn't work on iphone we skip this and add the functionality to have
		// both a background image (that uses scaleXY)
		// and a foreground image (that uses no scaling).
		else if( property.equals( IX_WIDGET.MAW_IMAGE_BUTTON_SCALE_MODE ) )
		{
			ImageButton imageButton = (ImageButton) getView( );
			if( value.equals( "none" ) )
			{
				imageButton.setScaleType( ImageView.ScaleType.CENTER );
			}
			else if( value.equals( "scaleXY" ) )
			{
				imageButton.setScaleType( ImageView.ScaleType.FIT_XY );
			}
			else if( value.equals( "scalePreserveAspect" ) )
			{
				imageButton.setScaleType( ImageView.ScaleType.FIT_CENTER );
			} 
			else 
			{
				throw new InvalidPropertyValueException( property , value );
			}
			
			return true;
		}
		*/
		else
		{
			if( super.setProperty( property, value ) )
			{
				return true;
			}
			
			return false;
		}

		return true;
	}
}