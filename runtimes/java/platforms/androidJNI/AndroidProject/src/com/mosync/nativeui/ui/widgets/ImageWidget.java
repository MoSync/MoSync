/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package com.mosync.nativeui.ui.widgets;

import java.io.File;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
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
	 * Image handle for MAW_BUTTON_IMAGE property;
	 */
	private int mImageHandle = 0;

	/**
	 * Image file path for MAW_IMAGE_PATH property;
	 */
	private String mImagePath = "";

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
			mImageHandle = IntConverter.convert( value );
			Bitmap image = NativeUI.getBitmap( mImageHandle );
			imageView.setImageBitmap( image );
			mImagePath = "";
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
		else if( property.equals( IX_WIDGET.MAW_IMAGE_PATH ) )
		{
			File imgFile = new File( value );
			if( imgFile.exists() )
			{
				mImagePath = value;
				mImageHandle = 0;
			    Bitmap myBitmap = BitmapFactory.decodeFile(imgFile.getAbsolutePath());
			    imageView.setImageBitmap(myBitmap);
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
		else if( property.equals( IX_WIDGET.MAW_IMAGE_IMAGE ) )
		{
			return Integer.toString(mImageHandle);
		}
		else if( property.equals( IX_WIDGET.MAW_IMAGE_PATH ) )
		{
			return mImagePath;
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
