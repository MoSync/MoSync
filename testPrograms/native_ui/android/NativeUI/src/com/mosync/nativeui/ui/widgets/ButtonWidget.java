package com.mosync.nativeui.ui.widgets;

import android.widget.Button;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * Wraps the behavior of a Button view. Some properties and event handling
 * are extensions of Widget.
 * 
 * @author fmattias
 */
public class ButtonWidget extends Widget
{
	
	public ButtonWidget(int handle, Button button)
	{
		super( handle, button );
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException
	{
		boolean propertySet = super.setProperty(property, value);
		if( propertySet )
		{
			return true;
		}
		
		Button button = (Button) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_TEXT ) )
		{
			button.setText( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_FONT_COLOR ) )
		{
			button.setTextColor( ColorConverter.convert( value ) );
		}
		else
		{
			return false;
		}
		
		return true;
	}

	
	
}
