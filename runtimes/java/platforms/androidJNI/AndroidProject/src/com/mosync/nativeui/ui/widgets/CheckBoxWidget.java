package com.mosync.nativeui.ui.widgets;

import android.widget.CheckBox;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A checkbox widget, that can be toggled and sends events when
 * it is clicked.
 * 
 * @author fmattias
 */
public class CheckBoxWidget extends Widget
{
	/**
	 * Constructor.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param checkbox A checkbox wrapped by this widget.
	 */
	public CheckBoxWidget(int handle, CheckBox checkBox)
	{
		super( handle, checkBox );
		// TODO Auto-generated constructor stub
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		CheckBox checkBox = (CheckBox) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_CHECKED ) )
		{
			boolean checked = BooleanConverter.convert( value );
			checkBox.setChecked( checked );
			
			return true;
		}
		else
		{
			return false;
		}
	}
}
