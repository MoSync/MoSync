package com.mosync.nativeui.ui.widgets;

import android.widget.Button;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * Wraps the behavior of a Button view. Some properties and event handling
 * are extensions of Widget.
 * 
 * @author fmattias
 */
public class ButtonWidget extends LabelWidget
{
	/**
	 * Constructor.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param button A button wrapped by this widget.
	 */
	public ButtonWidget(int handle, Button button)
	{
		super( handle, button );
	}
	
	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException, InvalidPropertyValueException
	{
		if( property.equals( IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR ) )
		{
			// Make sure bad values get caught, so that the API is consistent.
			ColorConverter.convert( value );
			
			// Ignore the background color of a button, since Android
			// buttons do not have anything behind the button foreground.
			return true;
		}
		else
		{
			return super.setProperty( property, value );
		}
	}
}
