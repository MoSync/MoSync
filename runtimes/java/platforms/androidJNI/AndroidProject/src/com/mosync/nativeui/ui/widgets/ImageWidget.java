package com.mosync.nativeui.ui.widgets;

import android.graphics.Bitmap;
import android.widget.ImageView;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
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
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		ImageView imageView = (ImageView) getView( );
		if( property.equals( IX_WIDGET.MAW_IMAGE_IMAGE ) )
		{
			Bitmap image = NativeUI.getBitmap( IntConverter.convert( value ) );
			imageView.setImageBitmap( image );
			return true;
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
			
			return true;
		}		
		else
		{
			return false;
		}
	}
}
