package com.mosync.nativeui.ui.widgets;

import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.KeyboardManager;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
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
		throws PropertyConversionException, InvalidPropertyValueException
	{
		if (super.setProperty(property, value))
		{
			return true;
		}

		ViewGroup view = (ViewGroup) getView();
		EditText editBox = (EditText) view.getChildAt(0);
		
		if (property.equals(IX_WIDGET.MAW_SEARCH_BAR_TEXT))
		{
			editBox.setText(value);
		}
		else if(property.equals(IX_WIDGET.MAW_SEARCH_BAR_PLACEHOLDER))
		{
			editBox.setHint(value);
		}
		else if(property.equals(IX_WIDGET.MAW_SEARCH_BAR_SHOW_KEYBOARD))
		{
			boolean showKeyboard = BooleanConverter.convert( value );
			if( showKeyboard )
			{
				return KeyboardManager.showKeyboardFor( getView( ) );
			}
			else
			{
				return KeyboardManager.hideKeyboardFor( getView( ) );
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
		
		if (property.equals(IX_WIDGET.MAW_SEARCH_BAR_TEXT))
		{
			return editBox.getText().toString();
		}
		else
		{
			return super.getProperty(property);
		}
	}
}
