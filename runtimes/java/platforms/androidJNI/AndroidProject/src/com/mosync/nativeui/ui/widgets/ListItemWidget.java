package com.mosync.nativeui.ui.widgets;

import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A class that represents a standardized list element, that
 * contains a label and an optional icon.
 * 
 * @author fmattias
 */
public class ListItemWidget extends Layout
{
	/**
	 * Label in the list layout.
	 */
	private TextView m_label = null;
	
	/**
	 * Icon in the list layout.
	 */
	private ImageView m_icon = null;

	/**
	 * 
	 * @param handle
	 * @param layout
	 * @param label
	 * @param icon
	 */
	public ListItemWidget(int handle, ViewGroup layout, TextView label, ImageView icon)
	{
		super( handle, layout );
		
		m_label = label;
		m_icon = icon;
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException
	{
		if( property.equals( Types.WIDGET_PROPERTY_TEXT ) )
		{
			m_label.setText( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_ICON ) )
		{
			int imageHandle = IntConverter.convert( value );
			m_icon.setImageBitmap( NativeUI.getBitmap( imageHandle ) );
		}
		else
		{
			return super.setProperty( property, value );
		}
		
		return true;
	}
	
	
	
}