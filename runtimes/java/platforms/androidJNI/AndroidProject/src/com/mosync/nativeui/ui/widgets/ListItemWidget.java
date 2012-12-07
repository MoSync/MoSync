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
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncFont.MoSyncFontHandle;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.FloatConverter;
import com.mosync.nativeui.util.properties.HorizontalAlignment;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.nativeui.ui.widgets.ListLayout.ITEM_VIEW_TYPE_ITEM;

/**
 * A class that represents a standardized list element, that
 * contains a label and an optional icon.
 *
 * @author fmattias
 */
public class ListItemWidget extends Layout
{
	/**
	 * Label in the list layout.
	 */
	private TextView m_label = null;

	/**
	 * Icon in the list layout.
	 */
	private ImageView m_icon = null;

	/**
	 * Standard list view item, or part of SegmentedListViewSection.
	 * ITEM_VIEW_TYPE_ITEM / ITEM_VIEW_TYPE_HEADER / ITEM_VIEW_TYPE_FOOTER
	 */
	private int mItemType = ITEM_VIEW_TYPE_ITEM;

	/**
	 *
	 * @param handle
	 * @param layout
	 * @param label
	 * @param icon
	 */
	public ListItemWidget(int handle, ViewGroup layout, TextView label, ImageView icon)
	{
		super( handle, layout );

		m_label = label;
		m_icon = icon;
		mItemType = ITEM_VIEW_TYPE_ITEM;
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( property.equals( IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT ) )
		{
			m_label.setText( value );
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_ITEM_ICON ) )
		{
			int imageHandle = IntConverter.convert( value );
			m_icon.setImageBitmap( NativeUI.getBitmap( imageHandle ) );
		}
		else if ( property.equals( IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_COLOR ) )
		{
			m_label.setTextColor( ColorConverter.convert(value) );
		}
		else if (property.equals( IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE ) )
		{
			m_label.setTextSize(TypedValue.COMPLEX_UNIT_PX, FloatConverter.convert(value) );
		}
		else if( property.equals(IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_HANDLE) )
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
		else if( property.equals(Types.WIDGET_PROPERTY_HORIZONTAL_ALIGNMENT))
		{
			alignLabelHorizontally(value);
		}
		else
		{
			return super.setProperty( property, value );
		}

		return true;
	}

	@Override
	public String getProperty(String property)
	{
		if( property.equals( IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT ) )
		{
			return m_label.getText().toString();
		}
		else if( property.equals(IX_WIDGET.MAW_LIST_VIEW_ITEM_IS_SELECTED) )
		{
			if ( getParent() instanceof ListViewSection )
			{
				// Get the parent section of this item.
				ListViewSection section = (ListViewSection) getParent();
				// Get the parent list of the section.
				ListLayout list = (ListLayout) section.getParent();
				return String.valueOf ( getHandle() == list.getSelectedItem() );
			}
			else if ( getParent() instanceof ListLayout )
			{
				ListLayout list = (ListLayout) getParent();
				return String.valueOf( getHandle() == list.getSelectedItem() );
			}
		}
		return super.getProperty( property );
	}

	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new RelativeLayout.LayoutParams( mosyncLayoutParams.getWidth( ), mosyncLayoutParams.getHeight( ) );
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
		m_label.setTypeface(aTypeface);
		m_label.setTextSize(aSize);
	}

	/**
	 * Set item type: default, header or footer.
	 * This method is used only for Segmented list views.
	 * @param itemType The item type.
	 */
	public void setItemType(int itemType)
	{
		mItemType = itemType;
	}

	/**
	 * Get the item type: default, header or footer.
	 * This method is used only for Segmented list views.
	 * @return The item type.
	 */
	public int getItemType()
	{
		return mItemType;
	}

	/**
	 * Align the label inside this list view item.
	 * @param alignment The alignment.
	 */
	public void alignLabelHorizontally(String alignment)
	{
		m_label.setGravity( HorizontalAlignment.convert( alignment ) );
	}

}