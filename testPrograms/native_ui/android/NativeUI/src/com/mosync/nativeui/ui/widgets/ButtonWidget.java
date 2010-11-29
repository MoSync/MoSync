package com.mosync.nativeui.ui.widgets;

import com.mosync.nativeui.core.Types;

import android.widget.Button;

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

	@Override
	public boolean setProperty(int property, String value)
	{
		boolean propertySet = super.setProperty(property, value);
		if( propertySet )
		{
			return true;
		}
		
		if( property == Types.WIDGET_PROPERTY_TEXT )
		{
			Button button = (Button) getView( );
			button.setText( value );
			return true;
		}
		
		return false;
	}

	
	
}
