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
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.ViewGroup;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A screen widget that represents the root of a widget
 * tree.
 *
 * Note: For now a screen widget is just a wrapper to a view
 * like all other widgets, but by default it fills the whole
 * screen.
 *
 * @author fmattias
 */
public class ScreenWidget extends Layout
{
	/**
	 * Title of this screen.
	 */
	private String m_title = "";

	/**
	 * Icon of this screen.
	 */
	private Drawable m_icon = null;

	/**
	 * Listener for when the title changes.
	 */
	private TitleChangedListener m_titleChangedListener = null;

	/**
	 * Constructor
	 *
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A screen wrapped by this widget.
	 */
	public ScreenWidget(int handle, ViewGroup view)
	{
		super( handle, view );
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}

		if( property.equals( IX_WIDGET.MAW_SCREEN_TITLE ) )
		{
			m_title = value;
			if( m_titleChangedListener != null )
			{
				m_titleChangedListener.titleChanged( this, m_title );
			}
		}
		else if( property.equals( IX_WIDGET.MAW_SCREEN_ICON ) )
		{
			int imageHandle = IntConverter.convert( value );
			Bitmap icon = NativeUI.getBitmap( imageHandle );
			if( icon != null )
			{
				m_icon = new BitmapDrawable( NativeUI.getBitmap( imageHandle ) );
			}
			else
			{
				throw new InvalidPropertyValueException( value, property );
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	/**
	 * @see Layout.updateLayoutParams.
	 */
	@Override
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new android.widget.FrameLayout.LayoutParams( mosyncLayoutParams.getWidth( ) , mosyncLayoutParams.getHeight( ) );
	}

	/**
	 * Returns the title of this screen.
	 *
	 * @return the title of this screen.
	 */
	public String getTitle()
	{
		return m_title;
	}

	/**
	 * Returns the icon of this screen.
	 *
	 * @return the icon of this screen.
	 */
	public Drawable getIcon()
	{
		return m_icon;
	}

	/**
	 * Sets a listener that is called when the title
	 * of the screen has changed.
	 *
	 * @param titleChangedListener A listener for title changes.
	 */
	public void setTitleChangedListener(TitleChangedListener titleChangedListener)
	{
		m_titleChangedListener = titleChangedListener;
	}

	/**
	 * A listener class for when the title of a screen changes,
	 * so that the for example the tab screen has a chance
	 * to update its tab title.
	 *
	 * @author fmattias
	 */
	public interface TitleChangedListener
	{
		/**
		 * Called when a title of the screen has changed.
		 *
		 * @param screen The screen whose title has changed.
		 * @param newTitle The new title of the screen.
		 */
		void titleChanged(ScreenWidget screen, String newTitle);
	}
}