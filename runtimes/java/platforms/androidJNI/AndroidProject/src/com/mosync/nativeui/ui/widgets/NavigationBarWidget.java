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

import android.graphics.Bitmap;
import android.graphics.Typeface;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.FloatConverter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A class for a linear layout that is horizontal.
 * It has two centered items: a text view and an icon.
 * Both are optional.
 *
 * @author emma
 */
public class NavigationBarWidget extends Widget
{

	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param linearLayout A linear layout wrapped by this widget.
	 */
	public NavigationBarWidget(int handle, android.widget.LinearLayout linearLayout)
	{
		super( handle, linearLayout );
		// Set fixed width for the nav bar.
		setProperty(IX_WIDGET.MAW_WIDGET_WIDTH , Integer.toString(IX_WIDGET.MAW_CONSTANT_FILL_AVAILABLE_SPACE));
		//TODO set screenHeight/10
		setProperty(IX_WIDGET.MAW_WIDGET_HEIGHT , "50");
		// Set a default blue color.
		setProperty(IX_WIDGET.MAW_WIDGET_BACKGROUND_GRADIENT , "0xC6E2FF,0x1874CD");
		// Set the default title color to blue.
		setProperty(IX_WIDGET.MAW_NAV_BAR_TITLE_FONT_COLOR , "0x104E8B");
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}

		ViewGroup view = (ViewGroup) getView();
		ImageView icon = (ImageView) view.getChildAt(0);
		TextView text = (TextView) view.getChildAt(1);

		if ( property.equals( IX_WIDGET.MAW_NAV_BAR_TITLE ))
		{
			if ( value.length() > 0 )
			{
				text.setVisibility(View.VISIBLE);
				text.setText(value);
			}
			else
			{
				text.setVisibility(View.GONE);
			}
		}
		else if ( property.equals( IX_WIDGET.MAW_NAV_BAR_ICON ) )
		{
			int imageHandle = IntConverter.convert( value );
			Bitmap bitmap = NativeUI.getBitmap( imageHandle );
			if( bitmap != null )
			{
				icon.setImageBitmap(bitmap);
				icon.setVisibility(View.VISIBLE);
			}
			else
			{
				throw new InvalidPropertyValueException( value, property );
			}
		}
		else if( property.equals( IX_WIDGET.MAW_NAV_BAR_TITLE_FONT_COLOR ) )
		{
			text.setTextColor( ColorConverter.convert( value ) );
		}
		else if( property.equals( IX_WIDGET.MAW_NAV_BAR_TITLE_FONT_SIZE ) )
		{
			text.setTextSize( FloatConverter.convert( value ) );
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
		TextView text = (TextView) view.getChildAt(1);

		if( property.equals( IX_WIDGET.MAW_NAV_BAR_TITLE ) )
		{
			return text.getText().toString();
		}
		else
		{
			return super.getProperty( property );
		}
	}

	/**
	 * Sets the font typeface from mosync nativeUI.
	 * It is called instead of the setProperty method,
	 * as two params are needed.
	 * @param aTypeface The typeface of MoSyncFontHandle
	 * @param aSize The size of MoSyncFontHandle
	 */
	public void setTitleFontTypeface(Typeface aTypeface, float aSize)
	{
		ViewGroup view = (ViewGroup) getView();
		TextView title = (TextView) view.getChildAt(0);
		title.setTypeface(aTypeface);
		title.setTextSize(aSize);
	}
}
