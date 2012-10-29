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

import android.graphics.Typeface;
import android.util.TypedValue;
import android.view.Gravity;
import android.widget.TextView;

import com.mosync.internal.android.MoSyncFont;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncFont.MoSyncFontHandle;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.FloatConverter;
import com.mosync.nativeui.util.properties.HorizontalAlignment;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;
import com.mosync.nativeui.util.properties.VerticalAlignment;

/**
 * This class represents a static text label, i.e.
 * not interactive.
 *
 * @author fmattias
 */
public class LabelWidget extends Widget
{

	/**
	 * Hold the value of the maximum number of lines for the text view.
	 */
	private int m_maxNrLines = 0;

	/**
	 * The current horizontal text alignment. Defaults to CENTER_HORIZONTAL.
	 * This value is needed because when we set a new value, we need
	 * to bit-or the horizontal and vertical components.
	 */
	private int mHorizontalGravity = Gravity.CENTER_HORIZONTAL;

	/**
	 * The current vertical text alignment. Defaults to TOP.
	 * This value is needed because when we set a new value, we need
	 * to bit-or the horizontal and vertical components.
	 */
	private int mVerticalGravity = Gravity.TOP;

	/**
	 * Constructor.
	 *
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A text view wrapped by this widget.
	 */
	public LabelWidget(int handle, TextView view)
	{
		super( handle, view );
		view.setGravity(mHorizontalGravity | mVerticalGravity);
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
		throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}

		TextView textView = (TextView) getView( );
		if( property.equals( IX_WIDGET.MAW_LABEL_TEXT ) )
		{
			textView.setText( value );
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_PLACEHOLDER ) )
		{
			textView.setHint( value );
		}
		else if( property.equals( IX_WIDGET.MAW_LABEL_FONT_COLOR ) )
		{
			textView.setTextColor( ColorConverter.convert( value ) );
		}
		else if( property.equals( IX_WIDGET.MAW_LABEL_FONT_SIZE ) )
		{
			textView.setTextSize( TypedValue.COMPLEX_UNIT_PX, FloatConverter.convert( value ) );
		}
		else if (property.equals(Types.WIDGET_PROPERTY_TEXT_HORIZONTAL_ALIGNMENT))
		{
			mHorizontalGravity = HorizontalAlignment.convert(value);
			textView.setGravity(mHorizontalGravity | mVerticalGravity);
		}
		else if (property.equals(Types.WIDGET_PROPERTY_TEXT_VERTICAL_ALIGNMENT))
		{
			mVerticalGravity = VerticalAlignment.convert(value);
			textView.setGravity(mHorizontalGravity | mVerticalGravity);
		}
		else if( property.equals(IX_WIDGET.MAW_LABEL_MAX_NUMBER_OF_LINES ) )
		{
			if ( IntConverter.convert(value) < 0 )
			{
				throw new InvalidPropertyValueException(property, value);
			}
			// This overrides any height setting.
			// Makes the TextView be at most this many lines tall.
			m_maxNrLines = IntConverter.convert(value);
			if ( m_maxNrLines == 0 )
			{
				textView.setMaxLines(Integer.MAX_VALUE);

			}
			else
			{
				textView.setMaxLines( m_maxNrLines );
			}
		}
		else if( property.equals( IX_WIDGET.MAW_LABEL_FONT_HANDLE ) )
		{
			MoSyncFontHandle currentFont = null;

			// Search the handle in the list of fonts.
			currentFont = MoSyncThread.getMoSyncFont(IntConverter .convert(value));

			if ( currentFont != null )
			{
				setFontTypeface(currentFont.getTypeface(), currentFont.getFontSize());
			}
			else
				throw new InvalidPropertyValueException(property, value);
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
		TextView textView = (TextView) getView( );
		if( property.equals( IX_WIDGET.MAW_LABEL_TEXT ) )
		{
			if( textView.getText( ) != null && textView.getText( ).length( ) > 0 )
			{
				return textView.getText( ).toString( );
			}
			else if( textView.getHint( ) != null && textView.getText( ).length( ) > 0 )
			{
				return textView.getHint( ).toString( );
			}
			else
			{
				return "";
			}
		}
		else if( property.equals(IX_WIDGET.MAW_LABEL_MAX_NUMBER_OF_LINES ) )
		{
			// If max number lines was not set, will return the default 0.
			return Integer.toString( m_maxNrLines );
		}
		else
		{
			return super.getProperty( property );
		}
	}

	/**
	 * Sets the font typeface from mosync nativeUI.
	 * It is called from the setProperty method,
	 *
	 * @param aTypeface The typeface of MoSyncFontHandle
	 * @param aSize The size of MoSyncFontHandle
	 * @return True if the widget supports font setting, false otherwise.
	 */
	@Override
	public void setFontTypeface(Typeface aTypeface, float aSize)
	{
		TextView textView = (TextView) getView( );
		textView.setTypeface(aTypeface);
		textView.setTextSize(aSize);
	}

}
