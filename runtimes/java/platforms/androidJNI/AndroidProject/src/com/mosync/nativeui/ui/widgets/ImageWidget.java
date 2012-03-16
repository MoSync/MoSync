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

import android.graphics.Bitmap;
import android.widget.ImageView;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.util.properties.FloatConverter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * Wraps the behavior of an Image view.
 *
 * @author fmattias
 */
public class ImageWidget extends Widget
{
	/**
	 * Constructor.
	 *
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view An image view wrapped by this widget.
	 */
	public ImageWidget(int handle, ImageView imageView)
	{
		super( handle, imageView );

		/**
		 * Defaults to no scaling.
		 */
		imageView.setScaleType(ImageView.ScaleType.CENTER);
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		ImageView imageView = (ImageView) getView( );
		if ( property.equals( IX_WIDGET.MAW_WIDGET_ALPHA ) )
		{
			float alpha = FloatConverter.convert( value );
			m_alpha = (int) (alpha * 255.0f);
			imageView.setAlpha(m_alpha);
			return true;
		}

		if( super.setProperty(property, value) )
		{
			return true;
		}

		if( property.equals( IX_WIDGET.MAW_IMAGE_IMAGE ) )
		{
			Bitmap image = NativeUI.getBitmap( IntConverter.convert( value ) );
			imageView.setImageBitmap( image );
		}
		else if( property.equals( IX_WIDGET.MAW_IMAGE_SCALE_MODE ) )
		{
			if( value.equals( "none" ) )
			{
				imageView.setScaleType( ImageView.ScaleType.CENTER );
			}
			else if( value.equals( "scaleXY" ) )
			{
				imageView.setScaleType( ImageView.ScaleType.FIT_XY );
			}
			else if( value.equals( "scalePreserveAspect" ) )
			{
				imageView.setScaleType( ImageView.ScaleType.FIT_CENTER );
			}
			else
			{
				throw new InvalidPropertyValueException( property , value );
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
//		ImageView imageView = (ImageView) getView( );
		if( property.equals(IX_WIDGET.MAW_WIDGET_ALPHA ) )
		{
			return Integer.toString(m_alpha);
			// TODO Keep it and call it for API level 11.
//			return Float.toString(imageView.getAlpha());
		}
		else
		{
			return super.getProperty( property );
		}
	}

	/**
	 * The alpha is stored.
	 */
	private int m_alpha = 0xff;
}
