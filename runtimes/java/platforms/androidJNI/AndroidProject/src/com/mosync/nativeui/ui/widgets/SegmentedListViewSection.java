/* Copyright (C) 2012 MoSync AB

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

import java.util.ArrayList;

import android.app.Activity;
import android.util.Log;
import android.widget.ListView;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncFont.MoSyncFontHandle;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.factories.ViewFactory;
import com.mosync.nativeui.ui.widgets.ListItemWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.nativeui.ui.widgets.SegmentedListLayout.ITEM_VIEW_TYPE_HEADER;
import static com.mosync.nativeui.ui.widgets.SegmentedListLayout.ITEM_VIEW_TYPE_FOOTER;

/**
 * A Section is a container for ListItemWidgets.
 * When created it will automatically hold a header
 * and a footer layout which are in fact customizable
 * ListItemWidgets.
 * Widget properties cannot be set on a Section.
 *
 * @author emma
 */
public class SegmentedListViewSection extends Layout //todo extends Widget
{
	// Default header and footer appearance.
	static final String HEADER_DEFAULT_BACKGROUND_COLOR = "838B83";
	static final String FOOTER_DEFAULT_BACKGROUND_COLOR = "838B83";
	static final int HEADER_DEFAULT_FONT_SIZE = 20;
	static final int FOOTER_DEFAULT_FONT_SIZE = 15;

	/**
	 * Listener for when the items are removed.
	 */
	private AdapterChangedListener mAdapterListener = null;

	/**
	 * Section content: list view items, including default header and footer.
	 */
	private ArrayList<ListItemWidget> mItems = new ArrayList<ListItemWidget>();

	/**
	 * Constructor.
	 * Add by default a header and a footer layout.
	 * Set different background colors and align labels to center.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param listView
	 * @param activity
	 */
	public SegmentedListViewSection(
			int handle, ListView listView, Activity activity)
	{
		super( handle, listView );

		// We don't need to later reffer to those objects using the widget handles.
		Widget headerWidget = ViewFactory.createView(
				IX_WIDGET.MAW_LIST_VIEW_ITEM,
				activity,
				MoSyncThread.getInstance().nativeCreatePlaceholder());

		headerWidget.setProperty(
				IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR, HEADER_DEFAULT_BACKGROUND_COLOR);
		ListItemWidget headerItem = (ListItemWidget) headerWidget;
		headerItem.alignLabelHorizontally(IX_WIDGET.MAW_ALIGNMENT_CENTER);
		headerItem.setProperty(
				IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE, Integer.toString(HEADER_DEFAULT_FONT_SIZE));

		Widget footerWidget = ViewFactory.createView(
				IX_WIDGET.MAW_LIST_VIEW_ITEM,
				activity,
				MoSyncThread.getInstance().nativeCreatePlaceholder() );

		footerWidget.setProperty(
				IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR, FOOTER_DEFAULT_BACKGROUND_COLOR);
		ListItemWidget footerItem = (ListItemWidget) footerWidget;
		footerItem.alignLabelHorizontally(IX_WIDGET.MAW_ALIGNMENT_CENTER);
		footerItem.setProperty(
				IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE, Integer.toString(FOOTER_DEFAULT_FONT_SIZE));

		headerItem.setItemType(ITEM_VIEW_TYPE_HEADER);
		mItems.add(0, headerItem);
		footerItem.setItemType(ITEM_VIEW_TYPE_FOOTER);
		mItems.add(1, footerItem);

		// Add the header and footer to the children widget list.
		m_children.add( 0, headerItem );
		m_children.add( 1, footerItem );
	}

	@Override
	public int addChildAt(Widget child, int index)
	{
		if (!(child instanceof ListItemWidget))
		{
			Log.e( "MoSync",
					"maWidgetInsertChild: SegmentedListViewSection can only contain ListItemWidgets." );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}

		int listIndex = index;
		if( index == -1 )
		{
			listIndex = mItems.size()-1;
		}

		// Always add items before the footer.
		child.setParent(this);
		m_children.add( listIndex, child );
		mItems.add(listIndex, (ListItemWidget)child);

		// Notify parent Segmented list of new item.
		if (mAdapterListener != null)
			mAdapterListener.itemAdded((ListItemWidget)child, this, listIndex);

		return IX_WIDGET.MAW_RES_OK;
	}

	@Override
	public int removeChild(Widget child)
	{
		child.setParent(null);
		m_children.remove(child);
		mItems.remove((ListItemWidget) child);

		// Notify Segmented list's adapter.
		if (mAdapterListener != null)
			mAdapterListener.itemRemoved((ListItemWidget)child);

		return IX_WIDGET.MAW_RES_OK;
	}

	/**
	 * Remove all items from this section.
	 */
	public void emptySection()
	{
		mItems.clear();
		m_children.clear();
	}

	/**
	 * Get items count,including header and footer.
	 * @return The section item count.
	 */
	public int itemsCount()
	{
		return mItems.size();
	}

	/**
	 * Set the header label text.
	 * @param text
	 */
	public void setHeaderText(final String text)
	{
		mItems.get(0).setProperty(
				IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT, text);
	}

	/**
	 * Set the footer label text.
	 * @param text
	 */
	public void setFooterText(final String text)
	{
		mItems.get(mItems.size()-1).setProperty(
				IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT, text);
	}

	/**
	 * Get item inside section.
	 * @param position The item position.
	 * @return The ListItemWidget object.
	 */
	public ListItemWidget getItem(int position)
	{
		if ( position > mItems.size() )
			return null;
		return mItems.get(position);
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( property.equals( IX_WIDGET.MAW_SEGMENTED_LIST_VIEW_SECTION_HEADER ) )
		{
			setHeaderText(value);
		}
		else if( property.equals( IX_WIDGET.MAW_SEGMENTED_LIST_VIEW_SECTION_FOOTER ) )
		{
			setFooterText(value);
		}
		else if( property.equals( "headerBackground") )
		{
			mItems.get(0).setProperty(IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR, value);
		}
		else if( property.equals( "footerBackgrouond") )
		{
			mItems.get( mItems.size()-1).setProperty(IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR, value);
		}
		else if( property.equals( "headerFontColor") )
		{
			mItems.get(0).setProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_COLOR, value);
		}
		else if( property.equals( "headerFontSize") )
		{
			mItems.get(0).setProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE, value);
		}
		else if( property.equals( "headerFont") )
		{
			MoSyncFontHandle currentFont = null;

			// Search the handle in the list of fonts.
			currentFont = MoSyncThread.getMoSyncFont(IntConverter .convert(value));

			if ( currentFont != null )
			{
				mItems.get(0).setFontTypeface( currentFont.getTypeface(), currentFont.getFontSize() );
			}
			else
				throw new InvalidPropertyValueException(property, value);
		}
		else if( property.equals("headerAlignment") )
		{
			mItems.get(0).setProperty("horizontalAlignment", value); //todo see constant, and remove from Types
		}
		else
		{
			// No widget/layout properties are inherited.
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
		if( property.equals( IX_WIDGET.MAW_SEGMENTED_LIST_VIEW_SECTION_HEADER ) )
		{
			return mItems.get(0).getProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT);
		}
		else if( property.equals( IX_WIDGET.MAW_SEGMENTED_LIST_VIEW_SECTION_FOOTER ) )
		{
			return mItems.get(mItems.size()).getProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT);
		}
		else
		{
			return "";
		}
	}

	/**
	 * Sets a listener that is called when the items of the section
	 * are changed.
	 *
	 * @param adapterListener A listener for item changes.
	 */
	public void setAdapterListener(AdapterChangedListener adapterListener)
	{
		mAdapterListener = adapterListener;
	}

	/**
	 * A listener class for when the items of the section change,
	 * so that the adapter of the segmented list can be updated.
	 *
	 * emma
	 */
	public interface AdapterChangedListener
	{
		/**
		 * Called when an item from the section has removed.
		 *
		 * @param item The item that was removed from the section.
		 */
		void itemRemoved(ListItemWidget item);

		/**
		 * called when an item was added to the section.
		 *
		 * @param item The new item.
		 * @param section The section.
		 * @param index The item index in section.
		 */
		void itemAdded(ListItemWidget item, SegmentedListViewSection section, int index);
	}
}