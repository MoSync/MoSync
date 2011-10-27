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
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * This class represents an editable text area.
 *
 * @author fmattias
 */
public class EditBoxWidget extends LabelWidget
{
	private int m_inputConstraints = 0x00000;

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
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_INPUT_MODE ) )
		{
			int constraints = IntConverter.convert(value);
			switch( constraints )
			{
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_ANY:
				m_inputConstraints |=
				InputType.TYPE_CLASS_TEXT |
				InputType.TYPE_TEXT_FLAG_MULTI_LINE;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_DECIMAL:
				m_inputConstraints |=
					InputType.TYPE_CLASS_NUMBER |
					InputType.TYPE_NUMBER_FLAG_DECIMAL |
					InputType.TYPE_NUMBER_FLAG_SIGNED;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_EMAILADDR:
				m_inputConstraints |=
					InputType.TYPE_CLASS_TEXT |
					InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_NUMERIC:
				m_inputConstraints |=
					InputType.TYPE_CLASS_NUMBER |
					InputType.TYPE_NUMBER_FLAG_SIGNED;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_PHONENUMBER:
				m_inputConstraints |= InputType.TYPE_CLASS_PHONE;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_SINGLE_LINE:
				m_inputConstraints |= InputType.TYPE_CLASS_TEXT;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_URL:
				m_inputConstraints |=
					InputType.TYPE_CLASS_TEXT |
					InputType.TYPE_TEXT_VARIATION_URI;
				break;
				default:
					throw new InvalidPropertyValueException(property, value);
			}
			editTextView.setInputType(m_inputConstraints);
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_INPUT_FLAG ) )
		{
			int constraints = IntConverter.convert(value);
			switch(constraints)
			{
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_PASSWORD:
				m_inputConstraints |= InputType.TYPE_TEXT_VARIATION_PASSWORD;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_SENSITIVE:
				m_inputConstraints |= InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE:
				m_inputConstraints |= InputType.TYPE_TEXT_FLAG_CAP_SENTENCES;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD:
				m_inputConstraints |= InputType.TYPE_TEXT_FLAG_CAP_WORDS;
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS:
				m_inputConstraints |= InputType.TYPE_TEXT_FLAG_CAP_CHARACTERS;
				break;
				default:
					throw new InvalidPropertyValueException(property, value);
			}

			editTextView.setInputType(m_inputConstraints);
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
			if ( editTextView.getText() != null )
			{
				return editTextView.getText().toString( );
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
