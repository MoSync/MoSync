package com.mosync.nativeui.ui.widgets;

import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.graphics.Bitmap;
import android.widget.ImageView;

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
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		ImageView imageView = (ImageView) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_IMAGE ) )
		{
			Bitmap image = NativeUI.getBitmap( IntConverter.convert( value ) );
			imageView.setImageBitmap( image );
			return true;
		}
		else
		{
			return false;
		}
	}
}
