package com.mosync.nativeui.ui.widgets;

import android.content.Context;
import android.os.IBinder;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.ColorConverter;
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
		else if(property.equals(Types.WIDGET_PROPERTY_SHOW_KEYBOARD))
		{
			boolean showKeyboard = BooleanConverter.convert( value );
			
   			InputMethodManager manager = (InputMethodManager)
			getView( ).getContext( ).getSystemService( Context.INPUT_METHOD_SERVICE );
			IBinder windowToken = editBox.getWindowToken();
			if (null == manager || null == windowToken)
			{
				return false;
			}
			
			if( showKeyboard )	
			{
				editBox.requestFocus( );
				manager.showSoftInput( editBox, InputMethodManager.SHOW_IMPLICIT );
			}
			else
			{
				manager.hideSoftInputFromWindow( editBox.getWindowToken(), 0);
			}
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
