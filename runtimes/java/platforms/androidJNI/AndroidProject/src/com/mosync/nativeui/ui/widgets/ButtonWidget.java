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

import android.widget.Button;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * Wraps the behavior of a Button view. Some properties and event handling
 * are extensions of Widget.
 *
 * @author fmattias
 */
public class ButtonWidget extends LabelWidget
{
	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param button A button wrapped by this widget.
	 */
	public ButtonWidget(int handle, Button button)
	{
		super( handle, button );
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException, InvalidPropertyValueException
	{
		Button button = (Button) getView( );

		if( property.equals( IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR ) )
		{
			// Apply the background color.
			return super.setProperty(property, value);
			// Make sure bad values get caught, so that the API is consistent.
			//ColorConverter.convert( value );

			// Ignore the background color of a button, since Android
			// buttons do not have anything behind the button foreground.
		}
		else if( property.equals( IX_WIDGET.MAW_BUTTON_TEXT ) )
		{
			button.setText( value );
		}
		else
		{
			return super.setProperty( property, value );
		}
		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		Button button = (Button) getView( );
		if( property.equals( IX_WIDGET.MAW_BUTTON_TEXT ) )
		{
			return button.getText().toString();
		}
		else
		{
			return super.getProperty( property );
		}
	}

}
