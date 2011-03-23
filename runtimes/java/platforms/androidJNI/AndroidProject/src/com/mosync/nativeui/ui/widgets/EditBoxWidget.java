package com.mosync.nativeui.ui.widgets;

import android.text.InputType;
import android.widget.EditText;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.KeyboardManager;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * This class represents an editable text area.
 * 
 * @author fmattias
 */
public class EditBoxWidget extends LabelWidget
{
	/**
	 * Constructor.
	 * 
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view An editable text view wrapped by this widget.
	 */
	public EditBoxWidget(int handle, EditText editView)
	{
		super( handle, editView );
		// TODO Auto-generated constructor stub
	}
	
	/**
	 * @see LabelWidget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		EditText editTextView = (EditText) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_EDIT_MODE ) )
		{
			if( value.equals( "password" ) )
			{
				editTextView.setInputType( InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD );
			}
			else if( value.equals( "text" ) )
			{
				editTextView.setInputType( InputType.TYPE_CLASS_TEXT );
			}
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_SHOW_KEYBOARD ) )
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
}
