package com.mosync.nativeui.ui.widgets;

import android.widget.TextView;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

public class LabelWidget extends Widget
{
	public LabelWidget(int handle, TextView view)
	{
		super(handle, view);
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		TextView textView = (TextView) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_TEXT ) )
		{
			textView.setText( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_FONT_COLOR ) )
		{
			textView.setTextColor( ColorConverter.convert( value ) );
		}
		else
		{
			return false;
		}
		
		return true;
	}
}
