package com.mosync.nativeui.ui.widgets;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.content.Context;
import android.os.IBinder;
import android.text.InputType;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

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
	public boolean setProperty(String property, String value) throws PropertyConversionException
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
			
   			InputMethodManager manager = (InputMethodManager)
			getView( ).getContext( ).getSystemService( Context.INPUT_METHOD_SERVICE );
			IBinder windowToken = editTextView.getWindowToken( );
			if( null == manager || null == windowToken )
			{
				return false;
			}
			
			if( showKeyboard )	
			{
				// Seems that it needs to have focus before we can show the keyboard.
				editTextView.requestFocus( );
				manager.showSoftInput( editTextView, InputMethodManager.SHOW_FORCED );
			}
			else
			{
				manager.hideSoftInputFromWindow( editTextView.getWindowToken( ), 0 );
			}
		}
		else
		{
			return false;
		}
		
		return true;

	}
}
