package com.mosync.nativeui.ui.widgets;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.widget.ImageButton;

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
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}

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
		else
		{
			return false;
		}

		return true;
	}
}