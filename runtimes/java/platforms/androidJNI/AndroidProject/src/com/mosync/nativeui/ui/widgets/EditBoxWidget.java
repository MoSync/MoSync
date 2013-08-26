/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package com.mosync.nativeui.ui.widgets;

import android.text.InputFilter;
import android.text.InputType;
import android.view.Gravity;
import android.widget.EditText;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.KeyboardManager;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.ColorConverter;
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
	private int m_inputFlagConstraints = 0x00000;
	private int m_inputModeContraints = 0x00000;
	private boolean m_multiline = false;

	/**
	 * Constructor.
	 *
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view An editable text view wrapped by this widget.
	 */
	public EditBoxWidget(int handle, EditText editView)
	{
		super( handle, editView );
		editView.setGravity( Gravity.LEFT );
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
				m_inputModeContraints =
				InputType.TYPE_CLASS_TEXT |
				InputType.TYPE_TEXT_FLAG_MULTI_LINE;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_DECIMAL:
				m_inputModeContraints =
					InputType.TYPE_CLASS_NUMBER |
					InputType.TYPE_NUMBER_FLAG_DECIMAL |
					InputType.TYPE_NUMBER_FLAG_SIGNED;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_EMAILADDR:
				m_inputModeContraints =
					InputType.TYPE_CLASS_TEXT |
					InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_NUMERIC:
				m_inputModeContraints =
					InputType.TYPE_CLASS_NUMBER |
					InputType.TYPE_NUMBER_FLAG_SIGNED;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_PHONENUMBER:
				m_inputModeContraints = InputType.TYPE_CLASS_PHONE;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_SINGLE_LINE:
				m_inputModeContraints = InputType.TYPE_CLASS_TEXT;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_TYPE_URL:
				m_inputModeContraints =
					InputType.TYPE_CLASS_TEXT |
					InputType.TYPE_TEXT_VARIATION_URI;
				break;
				default:
					throw new InvalidPropertyValueException(property, value);
			}
			if ( m_multiline )
				m_inputModeContraints |= InputType.TYPE_TEXT_FLAG_MULTI_LINE;
			editTextView.setInputType(m_inputModeContraints | m_inputFlagConstraints);
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_INPUT_FLAG ) )
		{
			int constraints = IntConverter.convert(value);
			switch(constraints)
			{
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_PASSWORD:
				m_inputFlagConstraints = InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_SENSITIVE:
				m_inputFlagConstraints = InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE:
				m_inputFlagConstraints = InputType.TYPE_TEXT_FLAG_CAP_SENTENCES;
				break;
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD:
				m_inputFlagConstraints = InputType.TYPE_TEXT_FLAG_CAP_WORDS;
			case IX_WIDGET.MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS:
				m_inputFlagConstraints = InputType.TYPE_TEXT_FLAG_CAP_CHARACTERS;
				break;
				default:
					throw new InvalidPropertyValueException(property, value);
			}

			editTextView.setInputType(m_inputFlagConstraints | m_inputModeContraints);
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_LINES_NUMBER ) )
		{
			int lines = IntConverter.convert(value);
			if ( lines < 0 )
				throw new InvalidPropertyValueException(property, value);

			// Set multi line by default.
			m_inputModeContraints |= InputType.TYPE_TEXT_FLAG_MULTI_LINE;
			m_multiline = true;
			editTextView.setInputType(m_inputModeContraints | m_inputFlagConstraints);
			editTextView.setLines(lines);
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_MAX_LINES ) )
		{
			int lines = IntConverter.convert(value);
			if ( lines < 0 )
				throw new InvalidPropertyValueException(property, value);
			editTextView.setMaxLines(lines);
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_MIN_LINES ) )
		{
			int lines = IntConverter.convert(value);
			if ( lines < 0 )
				throw new InvalidPropertyValueException(property, value);
			editTextView.setMinLines(lines);
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_MAX_LENGTH ) )
		{
			int maxLength = IntConverter.convert(value);
			InputFilter[] FilterArray = new InputFilter[1];
			FilterArray[0] = new InputFilter.LengthFilter(maxLength);
			editTextView.setFilters(FilterArray);
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_FONT_COLOR ) )
		{
			editTextView.setTextColor( ColorConverter.convert( value ) );
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_PLACEHOLDER_FONT_COLOR ) )
		{
			editTextView.setHintTextColor( ColorConverter.convert( value ) );
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
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_LINES_NUMBER ) )
		{
			return Integer.toString(editTextView.getLineCount());
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_MAX_LENGTH ) )
		{
			InputFilter[] FilterArray = editTextView.getFilters();
			if ( FilterArray.length == 1 )
			{
				return FilterArray[0].toString();
			}
		}
		return super.getProperty( property );
	}
}
