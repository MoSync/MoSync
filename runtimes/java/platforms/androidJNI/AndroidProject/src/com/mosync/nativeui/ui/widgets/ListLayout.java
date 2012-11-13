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

import java.util.ArrayList;
import java.util.List;

import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.SectionIndexer;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.ui.widgets.ListViewSection.AdapterChangedListener;
import com.mosync.nativeui.util.LayoutParamsSetter;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * This class wraps a list that displays a list of views.
 * The intention of this class is display views that all
 * has the same structure, otherwise a linear layout
 * may be preferred.
 *
 * Update: it can now contain List View Sections.
 * Therefore, 2 more list types are needed:
 *  - Segmented ( custom list view with default header
 *  and footer layouts).
 *  - Alphabetical ( alphabetical indexed list view).
 * Based on the list type, sections can be created accordingly.
 *
 * @author fmattias
 */
public class ListLayout extends Layout
	implements AdapterChangedListener
{
	/**
	 * Item types for segmented list view.
	 */
	public static final int ITEM_VIEW_TYPE_ITEM = 0;
	public static final int ITEM_VIEW_TYPE_HEADER = 1;
	public static final int ITEM_VIEW_TYPE_FOOTER = 2;
	public static final int ITEM_VIEW_TYPE_COUNT = 3;

	/**
	 * The list type:
	 *  - MAW_LIST_VIEW_TYPE_DEFAULT
	 *  - MAW_LIST_VIEW_TYPE_ALPHABETICAL
	 *  - MAW_LIST_VIEW_TYPE_SEGMENTED
	 */
	private int mListType = IX_WIDGET.MAW_LIST_VIEW_TYPE_DEFAULT;

	/**
	 * The sections of the list.
	 */
	private ArrayList<ListViewSection> mSections =
			new ArrayList<ListViewSection>();

	/**
	 * Feeds the list with views.
	 * This adapter is used for default and segmented lists.
	 */
	ViewAdapter m_viewAdapter = new ViewAdapter( );

	/**
	 * The adapter for the alphabetical indexed list.
	 */
	AlphabeticalViewAdapter<ListItemWidget> mAlphabeticalViewAdapter;

	/**
	 * Widget handle, used for posting item clicked events.
	 */
	private int mWidgetHandle;
	/**
	 * Widget handle of the selected item.
	 */
	private int mSelectedItemHandle;

	/**
	 * Allow selection property on the list.
	 * If disabled, none of the adapter's items can be selected.
	 */
	private boolean mAllowSelection = true;

	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param listView A list wrapped by this widget.
	 */
	public ListLayout(int handle, ListView listView)
	{
		super( handle, listView );
		listView.setAdapter( m_viewAdapter );
		mWidgetHandle = handle;
		listView.setOnItemClickListener(
				new ListOnItemClickListener( handle ) );
		//listView.setFocusable(true);
	}

	@Override
	public int addChildAt(Widget child, int index)
	{
		if ( mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_DEFAULT )
		{
			if ( child instanceof ListViewSection )
			{
				Log.e( "MoSync",
				"maWidgetInsertChild: Default ListLayout cannot contain ListViewSections.");
				return IX_WIDGET.MAW_RES_INVALID_HANDLE;
			}
			child.setParent(this);
			updateLayoutParamsForChild(child);

			int listIndex = index;
			if( index == -1 )
			{
				listIndex = m_viewAdapter.getCount( );
			}

			m_children.add( listIndex, child );
			m_viewAdapter.addAt( child.getRootView( ), listIndex );
		}
		else
		{
			if ( !(child instanceof ListViewSection) )
			{
				Log.e( "MoSync",
				"maWidgetInsertChild: ListLayout can only contain ListViewSections.");
				return IX_WIDGET.MAW_RES_INVALID_HANDLE;
			}
			ListViewSection section = (ListViewSection) child;
			// Check list type against section type.
			if ( mListType != section.getSectionType() )
			{
				Log.e( "MoSync",
				"maWidgetInsertChild: List type does not match Section type.");
				return IX_WIDGET.MAW_RES_INVALID_HANDLE;
			}
			int listIndex = index;
			if( index == -1 )
			{
				listIndex = mSections.size();
			}
			// Add the section to the section list.
			section.setParent(this);
			m_children.add(listIndex, child);
			mSections.add(listIndex, section);

			if ( mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_SEGMENTED )
				addSegmentedSection(section, listIndex);
			else
			{
				mAlphabeticalViewAdapter.reloadPreviewLetters();
			}

			// Set adapter listeners for each section.
			section.setAdapterListener(this);
		}

		return IX_WIDGET.MAW_RES_OK;
	}

	/**
	 * Add a segmented section to the list.
	 * @param child
	 * @param index
	 * @return
	 */
	public void addSegmentedSection(ListViewSection section,int index)
	{
		int listIndex = index;
		if( index == -1 )
		{
			listIndex = m_viewAdapter.getCount( );
		}
		// Get all items from section and add them to the list adapter.
		for (int i=0; i < section.itemsCount(); i++)
		{
			ListItemWidget item = section.getItem(i);
			m_viewAdapter.addAt(item.getView(), listIndex + i);
		}
	}

	/**
	 * Add the child to the view adapter rather to
	 * the list view itself.
	 *
	 * @see Layout.addChild.
	 */
	@Override
	public int removeChild(Widget child)
	{
		if ( mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_DEFAULT )
		{
			child.setParent(null);
			m_children.remove( child );
			m_viewAdapter.remove( child.getRootView( ) );
		}
		else if( mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_ALPHABETICAL ||
				mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_SEGMENTED)
		{
			// Do not call super.removeChild because section is more
			// of a container, that a widget.
			ListViewSection section = (ListViewSection) child;
			section.setParent(null);
			m_children.remove( child );
			mSections.remove(section);

			// Nullify adapter listener.
			section.setAdapterListener(null);

			if ( mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_SEGMENTED )
				removeSegmentedSection(section);
			else
				mAlphabeticalViewAdapter.reloadPreviewLetters();
		}

		return IX_WIDGET.MAW_RES_OK;
	}

	/**
	 * Remove all items from the segmented section.
	 * @param section
	 */
	public void removeSegmentedSection(ListViewSection section)
	{
		for (int i=0; i < section.itemsCount(); i++)
		{
			// Section is removed from list, it's items removed from adapter.
			m_viewAdapter.remove(section.getItem(i).getView());
		}
		m_viewAdapter.notifyDataSetChanged();
	}

	/**
	 * @see Layout.updateLayoutParams.
	 */
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new AbsListView.LayoutParams( mosyncLayoutParams.getWidth( ) , mosyncLayoutParams.getHeight( ) );
	}

	/**
	 * Updates the Android layout params for the given child, according
	 * to the parameters specified in the child. Only the parameters that
	 * are supported for the layout will be taken into consideration.
	 *
	 * @param child The child for which to update the layout params.
	 */
	public void updateLayoutParamsForChild(Widget child)
	{
		// Set layout params for the child
		ViewGroup.LayoutParams nativeLayoutParams = createNativeLayoutParams( child.getLayoutParams( ) );
		LayoutParamsSetter.setPossibleParams( child.getLayoutParams( ), nativeLayoutParams );

		View childView = child.getView();

		View rootView = child.getRootView();
		if( childView != rootView )
		{
			if ( child instanceof Layout )
			{
				if ( !((Layout) child).isScrollable() )
				{
					rootView.setLayoutParams(
							new ViewGroup.LayoutParams( nativeLayoutParams.width, nativeLayoutParams.height ) );
				}
			}
		}
		else
		{
			childView.setLayoutParams( nativeLayoutParams );
		}
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}

		ListView listView = (ListView) getView();
		if ( property.equals(IX_WIDGET.MAW_LIST_VIEW_TYPE) )
		{
			if ( IntConverter.convert(value) == IX_WIDGET.MAW_LIST_VIEW_TYPE_DEFAULT)
			{
				listView.setAdapter( m_viewAdapter );
				mListType = IX_WIDGET.MAW_LIST_VIEW_TYPE_DEFAULT;
			}
			else if( IntConverter.convert(value) == IX_WIDGET.MAW_LIST_VIEW_TYPE_SEGMENTED )
			{
				mListType = IX_WIDGET.MAW_LIST_VIEW_TYPE_SEGMENTED;
				m_viewAdapter = new SegmentedViewAdapter( );
				listView.setAdapter(m_viewAdapter);
				listView.setOnItemClickListener(
						new SectionedListOnItemClickListener( mWidgetHandle ) );
			}
			else if( IntConverter.convert(value) == IX_WIDGET.MAW_LIST_VIEW_TYPE_ALPHABETICAL )
			{
				mListType = IX_WIDGET.MAW_LIST_VIEW_TYPE_ALPHABETICAL;
				mAlphabeticalViewAdapter = new AlphabeticalViewAdapter<ListItemWidget>( );
				listView.setAdapter( mAlphabeticalViewAdapter );
				listView.setFastScrollEnabled(true);
				listView.setOnItemClickListener(
						new SectionedListOnItemClickListener( mWidgetHandle ) );
			}
			else
			{
				throw new InvalidPropertyValueException(property, value);
			}
		}
		else if( property.equals(IX_WIDGET.MAW_LIST_VIEW_RELOAD_DATA) )
		{
			if ( mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_ALPHABETICAL )
				mAlphabeticalViewAdapter.reloadPreviewLetters();
			else if ( mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_SEGMENTED )
				m_viewAdapter.notifyDataSetChanged();
		}
		else if( property.equals( Types.WIDGET_PROPERTY_REVERSED ) )
		{
			m_viewAdapter.setReversed( BooleanConverter.convert( value ) );
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_ALLOW_SELECTION ) )
		{
			mAllowSelection = BooleanConverter.convert(value);
		}
		else if(property.equals( IX_WIDGET.MAW_LIST_VIEW_REQUEST_FOCUS ))
		{
			listView.setFocusable(true);
			listView.setFocusableInTouchMode(true);
			listView.requestFocus();
			listView.setSelection(0);
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
		if( property.equals( IX_WIDGET.MAW_LIST_VIEW_ALLOW_SELECTION ) )
		{
			return Boolean.toString(mAllowSelection);
		}
		else
		{
			return super.getProperty( property );
		}
	}

	/**
	 * Get the handle of the  last item that was selected in the list.
	 * @return The selected item widget handle.
	 */
	public int getSelectedItem()
	{
		return mSelectedItemHandle;
	}

	/**
	 * Class responsible for sending a mosync event when an item
	 * in the default list view has been clicked.
	 *
	 * @author fmattias
	 */
	public class ListOnItemClickListener implements OnItemClickListener
	{
		private int m_handle = -1;

		public ListOnItemClickListener(int handle)
		{
			m_handle = handle;
		}

		/**
		 * @see OnItemClickListener.onItemClick.
		 */
		@Override
		public void onItemClick(AdapterView<?> parent, View view, int position, long id)
		{
			// Store the last selected item.
			mSelectedItemHandle = m_children.get(position).getHandle();
			//mSelectedItem = (ListItemWidget) m_viewAdapter.getView(position, null, null);
			// Assume that the view's id is always the same as the handle.
			EventQueue.getDefault( ).postWidgetItemClickedEvent( m_handle, position );
		}
	}

	/**
	 * Class responsible for sending a mosync event when an item
	 * in the sectioned list has been clicked.
	 *
	 * @author emma
	 */
	public class SectionedListOnItemClickListener implements OnItemClickListener
	{
		private int m_handle = -1;

		public SectionedListOnItemClickListener(int handle)
		{
			m_handle = handle;
		}

		/**
		 * @see OnItemClickListener.onItemClick.
		 */
		@Override
		public void onItemClick(AdapterView<?> parent, View view, int position, long id)
		{
			int browseIndex = 0;
			int sectionIndex = 0;
			// Section's start position in list.
			int sectionPosition = 0;
			// Item's position in the section.
			int itemPosition = 0;

			// Get the index of the section that holds this item,
			// and it's start position.
			for (int i=0; i < mSections.size(); i++)
			{
				ListViewSection section = mSections.get(i);
				browseIndex += section.itemsCount();
				sectionIndex = i;
				if ( browseIndex > position )
				{
					itemPosition = position-sectionPosition;
					// Get the position inside the section, decrease it by 1 if the section has a header.
					if ( section.hasHeader() )
					{
						itemPosition --;
					}
					// Store the selected state of the item.
					mSelectedItemHandle = section.getItem(itemPosition).getHandle();

					// The index ignores the header.
					EventQueue.getDefault().
							postSegmentedListItemClicked(m_handle, sectionIndex, itemPosition);
					return;
				}
				sectionPosition += section.itemsCount();
			}
			EventQueue.getDefault().
					postSegmentedListItemClicked(m_handle, sectionIndex, itemPosition);
		}
	}

	/**
	 * See AdapterChangedListener.itemRemoved.
	 */
	@Override
	public void itemRemoved(ListItemWidget item)
	{
		if ( mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_ALPHABETICAL )
		{
			mAlphabeticalViewAdapter.notifyDataSetChanged();
		}
		else
		{
			m_viewAdapter.remove(item.getView());
		}

	}

	/**
	 * See AdapterChangedListener.itemAdded.
	 */
	@Override
	public void itemAdded(
			ListItemWidget item, ListViewSection section, int index)
	{
		if ( mListType == IX_WIDGET.MAW_LIST_VIEW_TYPE_ALPHABETICAL )
		{
			mAlphabeticalViewAdapter.notifyDataSetChanged();
		}
		else
		{
			// Based on index inside the section, compute the position in list.
			int itemPos = getItemPosition(mSections.indexOf(section), index);
			m_viewAdapter.addAt(item.getView(), itemPos);
		}
	}

	/**
	 * Helper function.
	 * Based on item position inside the section,
	 * return the position inside the list.
	 * @param sectionIndex The section index.
	 * @param index The index inside the section.
	 * @return The position inside the list.
	 */
	private int getItemPosition(int sectionIndex, int index)
	{
		int browseIndex = 0;
		if (sectionIndex == 0)
			return index;

		for (int i=1; i <= sectionIndex; i++)
		{
			browseIndex += mSections.get(i).itemsCount();
		}
		return browseIndex + index;
	}

	/**
	 * An adapter that feeds the list view with views.
	 * used for default list view type.
	 *
	 * @author fmattias
	 */
	class ViewAdapter extends BaseAdapter
	{
		/**
		 * The list of views contained in this adapter.
		 */
		List<View> m_views = new ArrayList<View>();

		/**
		 * Determines if the list items should be displayed in the
		 * reversed order or not.
		 */
		private boolean m_reversed = false;

		public ViewAdapter()
		{

		}

		/**
		 * Adds a view to the adapter at the specific index and notifies
		 * the listeners.
		 *
		 * @param view The view to add.
		 * @param index The index to add the element at.
		 */
		public void addAt(View view, int index)
		{
			if( view != null )
			{
				m_views.add( index, view );
				notifyDataSetChanged( );
			}
		}

		/**
		 * Removes a view from the adapter and notifies the listeners.
		 *
		 * @param view The view to remove.
		 */
		public void remove(View view)
		{
			if( view != null )
			{
				m_views.remove( view );
				notifyDataSetChanged( );
			}
		}

		/**
		 * Sets if the list items should be displayed in reversed order.
		 *
		 * @param reversed true means that the elements in the list will be in the
		 *                 reverse order, false means that they will be in the
		 *                 default order.
		 */
		public void setReversed(boolean reversed)
		{
			m_reversed = reversed;
			notifyDataSetChanged( );
		}

		/**
		 * @see BaseAdapter.getCount.
		 */
		@Override
		public int getCount()
		{
			return m_views.size( );
		}

		/**
		 * @see BaseAdapter.getItem.
		 */
		@Override
		public Object getItem(int position)
		{
			// We do not have data associated to the view, so
			// just return the position as the data.
			return position;
		}

		/**
		 * @see BaseAdapter.getItemId.
		 */
		@Override
		public long getItemId(int position)
		{
			return position;
		}

		/**
		 * @see BaseAdapter.getView.
		 */
		@Override
		public View getView(int position, View convertView, ViewGroup parent)
		{
			if( position < m_views.size( ) )
			{
				if( !m_reversed )
				{
					return m_views.get( position );
				}
				else
				{
					return m_views.get( m_views.size( ) - position - 1 );
				}
			}
			else
			{
				Log.i( "MoSync", "Returning old view." );
				return convertView;
			}
		}

		/**
		 * @see BaseAdapter.isEmpty.
		 */
		@Override
		public boolean isEmpty()
		{
			return m_views.isEmpty( );
		}

		/**
		 * @see BaseAdapter.isEnabled.
		 */
		@Override
		public boolean isEnabled(int position) {
			return mAllowSelection;
		}
	}

	/**
	 * A custom adapter that feeds the list with views:
	 * standard list view items and separators for
	 * header and footer.
	 *
	 * @author emma
	 */
	class SegmentedViewAdapter extends ViewAdapter
	{
		public SegmentedViewAdapter()
		{
			super();
		}

		/**
		 * @see BaseAdapter.isEnabled.
		 */
		@Override
		public boolean isEnabled(int position) {
			// A separator cannot be clicked !
			return (getItemViewType(position) == ITEM_VIEW_TYPE_ITEM && mAllowSelection) ;
		}

		/**
		 * @see BaseAdapter.getItemViewType.
		 */
		@Override
		public int getItemViewType(int position) {
			ListItemWidget item = (ListItemWidget) getItem(position);
			return item.getItemType();
		}

		/**
		 * @see BaseAdapter.getItem.
		 */
		@Override
		public Object getItem(int position) {
			int browseIndex = 0;
			int sectionIndex = 0;
			// Section start position in list.
			int sectionPosition = 0;

			// Get the index of the section that holds this item,
			// and get it's start position.
			for (int i=0; i < mSections.size(); i++)
			{
				ListViewSection section = mSections.get(i);
				browseIndex += section.itemsCount();
				sectionIndex = i;
				if ( browseIndex > position )
				{
					return section.getItem(position-sectionPosition);
				}
				sectionPosition += section.itemsCount();
			}
			return mSections.get(sectionIndex).getItem(position-sectionPosition);
		}
	}

	/**
	 * A custom adapter that feeds the alphabetical indexed list with views.
	 * Used only on alphabetical list types.
	 *
	 * @author emma
	 */
	class AlphabeticalViewAdapter<T>
			extends BaseAdapter
			implements SectionIndexer {

		/**
		 * Array object that holds all the preview letters.
		 */
        String[] mPreviewLetters;

        /**
         * Constructor.
         * When the adapter object is constructed, there are no
         * containing views.
         * When items are removed or added, the adapter
         * will be refreshed.
         */
		public AlphabeticalViewAdapter()
		{
			super();
		}

		/**
		 * Refresh all the preview letters,
		 * when a section is added or removed.
		 */
		public void reloadPreviewLetters()
		{
			ArrayList<String> keyList = new ArrayList<String>();
			for (int i=0; i < mSections.size(); i++)
			{
				keyList.add(mSections.get(i).mAlphabeticIndex);
			}
			mPreviewLetters = new String[keyList.size()];
			// Convert to an array of objects.
			keyList.toArray(mPreviewLetters);

			notifyDataSetChanged();
		}

		/**
		 * @see BaseAdapter.getView.
		 */
		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			ListItemWidget item = (ListItemWidget) getItem(position);
			return item.getRootView();
		}

		/**
		 * @see BaseAdapter.isEmpty.
		 */
		@Override
		public boolean isEmpty()
		{
			return mSections.isEmpty();
		}

		/**
		 * @see BaseAdapter.getCount.
		 */
		@Override
		public int getCount()
		{
			int totalCount = 0;
			for (int i=0; i < mSections.size(); i++)
			{
				totalCount += mSections.get(i).itemsCount();
			}

			return totalCount;
		}

		/**
		 * @see SectionIndexer.getSections.
		 */
		@Override
		public int getPositionForSection(int section) {
			int browseIndex = 0;
			for (int i=0; i < section; i++)
			{
				browseIndex += mSections.get(i).itemsCount();
			}
			return browseIndex;
		}

		/**
		 * @see SectionIndexer.getSections.
		 */
		@Override
		public int getSectionForPosition(int position) {
			return 0;
		}

		/**
		 * @see SectionIndexer.getSections.
		 */
		@Override
		public Object[] getSections() {
			// The list view will call toString() on each object
			// to get the preview letter.
			return mPreviewLetters;
		}

		/**
		 * @see BaseAdapter.getItem.
		 */
		@Override
		public Object getItem(int position) {
			//return getView(position, null,null);
			int browseIndex = 0;
			int sectionIndex = 0;
			// Section start position in list.
			int sectionPosition = 0;

			// Get the index of the section that holds this item,
			// and get it's start position.
			for (int i=0; i < mSections.size(); i++)
			{
				ListViewSection section = mSections.get(i);
				browseIndex += section.itemsCount();
				sectionIndex = i;
				if ( browseIndex > position )
				{
					return section.getItem(position-sectionPosition);
				}
				sectionPosition += section.itemsCount();
			}
			return mSections.get(sectionIndex).getItem(position-sectionPosition);
		}

		/**
		 * @see BaseAdapter.getItemId.
		 */
		@Override
		public long getItemId(int position)
		{
			return position;
		}

		/**
		 * @see BaseAdapter.isEnabled.
		 */
		@Override
		public boolean isEnabled(int position) {
			// A separator cannot be clicked !
			return (getItemViewType(position) == ITEM_VIEW_TYPE_ITEM && mAllowSelection);
		}

		/**
		 * @see BaseAdapter.getItemViewType.
		 */
		@Override
		public int getItemViewType(int position) {
			ListItemWidget item = (ListItemWidget) getItem(position);
			return item.getItemType();
		}
	}
}
