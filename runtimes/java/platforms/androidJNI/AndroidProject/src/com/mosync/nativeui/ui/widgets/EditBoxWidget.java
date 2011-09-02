/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

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
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_TEXT ) )
		{
			editTextView.setText( value );
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
		EditText editTextView = (EditText) getView( );
		if( property.equals( IX_WIDGET.MAW_EDIT_BOX_TEXT ) )
		{
			if ( editTextView.getText() != null && editTextView.getText().length() > 0 )
			{
				return editTextView.getText().toString( );
			}
			else if ( editTextView.getHint() != null && editTextView.getHint().length() > 0 )
			{
				return editTextView.getHint().toString( );
			}
			else
			{
				return "";
			}
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
