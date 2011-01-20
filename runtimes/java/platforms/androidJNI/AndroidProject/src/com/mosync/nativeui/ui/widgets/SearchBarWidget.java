package com.mosync.nativeui.ui.widgets;

import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.FloatConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A search bar with a text input field and a search icon button.
 * 
 * @author Mikael
 */
public class SearchBarWidget extends Widget
{
	/**
	 * Constructor.
	 * 
	 * @param handle A handle to this widget.
	 * @param view A search bar view wrapped by this widget.
	 */
	public SearchBarWidget(int handle, View view)
	{
		super(handle, view);
		//view.setGravity(0);
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value) 
		throws PropertyConversionException
	{
		if (super.setProperty(property, value))
		{
			return true;
		}

		ViewGroup view = (ViewGroup) getView();
		EditText editBox = (EditText) view.getChildAt(0);
		
		if (property.equals(Types.WIDGET_PROPERTY_TEXT))
		{
			editBox.setText(value);
		}
		else if(property.equals(Types.WIDGET_PROPERTY_FONT_COLOR))
		{
			editBox.setTextColor(ColorConverter.convert(value));
		}
		else if(property.equals(Types.WIDGET_PROPERTY_FONT_SIZE))
		{
			editBox.setTextSize(FloatConverter.convert(value));
		}
		else
		{
			return false;
		}
		
		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		ViewGroup view = (ViewGroup) getView();
		EditText editBox = (EditText) view.getChildAt(0);
		
		if (property.equals(Types.WIDGET_PROPERTY_TEXT))
		{
			return editBox.getText().toString();
		}
		else
		{
			return super.getProperty(property);
		}
	}
}
