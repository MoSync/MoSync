package com.mosync.nativeui.ui.widgets;

import android.widget.ToggleButton;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A toggle button widget, that displays checked/unchecked states as a button with a "light" indicator and 
 * by default accompanied with the text "ON" or "OFF".
 * Sends events when it is clicked.
 * 
 * @author emma tresanszki
 */
public class ToggleButtonWidget extends Widget
{
	/**
	 * Constructor.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param toggleBtn A toggle button wrapped by this widget.
	 */
	public ToggleButtonWidget(int handle, ToggleButton toggleBtn)
	{
		super( handle, toggleBtn );
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		ToggleButton toggleBtn = (ToggleButton) getView( );
		if( property.equals( IX_WIDGET.MAW_TOGGLE_BUTTON_CHECKED ) )
		{
			boolean checked = BooleanConverter.convert( value );
			toggleBtn.setChecked( checked );
			
			return true;
		}
		else
		{
			return false;
		}
	}
	
	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		if( property.equals( IX_WIDGET.MAW_TOGGLE_BUTTON_CHECKED ) )
		{
			ToggleButton toggleBtn = (ToggleButton) getView( );
			return Boolean.toString( toggleBtn.isChecked( ) );
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
