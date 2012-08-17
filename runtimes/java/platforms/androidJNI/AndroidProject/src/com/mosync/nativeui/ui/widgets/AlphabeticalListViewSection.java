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

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.ListItemWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.ui.widgets.SegmentedListViewSection.AdapterChangedListener;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;


/**
 * A Section is a container for ListItemWidgets.
 * Widget properties cannot be set on a Section.
 * Extend from layout so that maWidgetDestroy will destroy
 * all it's children.
 *
 * An alphabetical list view section can be parented only
 * by an AlphabeticalListLayout.
 *
 * @author emma
 */
public class AlphabeticalListViewSection extends Layout
{

	/**
	 * Listener for when the items are removed or added.
	 */
	private AdapterChangedListener mAdapterListener = null;

	/**
	 * Section content: list view items, including default header and footer.
	 */
	private ArrayList<ListItemWidget> mItems = new ArrayList<ListItemWidget>();

	/**
	 *
	 */
	public String mAlphabeticIndex;

	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param listView
	 * @param activity
	 */
	public AlphabeticalListViewSection(
			int handle, ListView listView, Activity activity)
	{
		super( handle, listView );
	}

	@Override
	public int addChildAt(Widget child, int index)
	{
		Log.e("@@MoSync","AplhabeticalListSection addChildAt, index " + index);
		if (!(child instanceof ListItemWidget))
		{
			Log.e( "MoSync",
					"maWidgetInsertChild: AlphabeticalListViewSection can only contain ListItemWidgets." );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}

		int listIndex = index;
		if( index == -1 )
		{
			listIndex = mItems.size();
		}

		Log.e("@@MoSync","AplhabeticalListSection addChildAt, listIndex " + listIndex);
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
		if( property.equals( IX_WIDGET.MAW_SEGMENTED_LIST_VIEW_SECTION_TITLE) ) // that is the alphabetical index
		{
			mAlphabeticIndex = value.substring(0,1).toUpperCase(); //or substr(0,1)
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
		if( property.equals( IX_WIDGET.MAW_SEGMENTED_LIST_VIEW_SECTION_TITLE ) )
		{
			return mAlphabeticIndex;
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
		void itemAdded(ListItemWidget item, AlphabeticalListViewSection section, int index);
	}
}