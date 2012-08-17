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
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.SectionIndexer;
import android.widget.AdapterView.OnItemClickListener;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.ui.widgets.AlphabeticalListViewSection.AdapterChangedListener;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * An alphabetical indexed list view.
 *
 * @author emma
 */
public class AlphabeticalListLayout extends ListLayout
	implements AdapterChangedListener
{
	/**
	 * The adapter for the indexed list.
	 */
	AlphabeticalViewAdapter<ListItemWidget> mViewAdapter;

	/**
	 * The sections of the alphabetical list.
	 */
	private ArrayList<AlphabeticalListViewSection> mSections =
			new ArrayList<AlphabeticalListViewSection>();

	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param listView A list wrapped by this widget.
	 */
	public AlphabeticalListLayout(int handle, Activity activity, ListView listView)
	{
		super( handle, listView );

        mViewAdapter = new AlphabeticalViewAdapter<ListItemWidget>();
		listView.setAdapter( mViewAdapter );
		listView.setFastScrollEnabled(true);

		listView.setOnItemClickListener( new AlphabeticalListOnItemClickListener( handle ) );
	}

	/**
	 * Class responsible for sending a mosync event when an item
	 * in the alphabetical list has been clicked.
	 *
	 * @author emma
	 */
	public class AlphabeticalListOnItemClickListener implements OnItemClickListener
	{
		private int m_handle = -1;

		public AlphabeticalListOnItemClickListener(int handle)
		{
			m_handle = handle;
		}

		/**
		 * @see OnItemClickListener.onItemClick.
		 */
		@Override
		public void onItemClick(AdapterView<?> parent, View view, int position, long id)
		{

			// Post event: position and section.
		}
	}

	/**
	 * Add the child to the view adapter rather to
	 * the list view itself.
	 */
	@Override
	public int addChildAt(Widget child, int index)
	{
		Log.e("@@MoSync","AplhabeticalListLayout addChildAt, index " + index);
		if ( !(child instanceof AlphabeticalListViewSection) )
		{
			Log.e( "MoSync",
					"maWidgetInsertChild: AlphabeticalListLayout can only contain AlphabeticalListViewSections.");
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}

		int listIndex = index;
		if( index == -1 )
		{
			listIndex = mSections.size();
		}

		AlphabeticalListViewSection section = (AlphabeticalListViewSection) child;
		section.setParent(this);
		m_children.add(child);
		mSections.add(listIndex, section);

		mViewAdapter.reloadPreviewLetters();

		// Set adapter listeners for each section.
		section.setAdapterListener(this);

		return IX_WIDGET.MAW_RES_OK;
	}

	@Override
	public int removeChild(Widget child)
	{
		Log.e("@@MoSync","AlphabeticalList removeChild");
		// Do not call super.removeChild because section is not a common widget.
		// The view doesn't need to be removed ftom the ViewGroup, or from the adapter itself.
		AlphabeticalListViewSection section = (AlphabeticalListViewSection) child;
		section.setParent(null);
		m_children.remove( child );
		mSections.remove(section);

		mViewAdapter.reloadPreviewLetters();

		// Nullify adapter listener.
		section.setAdapterListener(null);

		return IX_WIDGET.MAW_RES_OK;
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		// Reversed property is not available.
		if( super.setProperty( property, value ) &&
				!property.equals(Types.WIDGET_PROPERTY_REVERSED))
		{
			return true;
		}

		if( property.equals( "reload_data" ) )
		{
			mViewAdapter.notifyDataSetChanged();

		}
		else
		{
			return false;
		}

		return true;
	}

	@Override
	public void itemRemoved(ListItemWidget item)
	{
//		m_viewAdapter.remove(item.getRootView());
		mViewAdapter.notifyDataSetChanged();
	}

	@Override
	public void itemAdded(
			ListItemWidget item, AlphabeticalListViewSection section, int index)
	{
		// Based on index inside the section, compute the position in list.
//		int itemPos = getItemPosition(mSections.indexOf(section), index);
//		m_viewAdapter.addAt(item.getRootView(), itemPos);

		mViewAdapter.notifyDataSetChanged();
	}

	/**
	 * A custom adapter that feeds the alphabetical indexed list with views.
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
				Log.e("@@MoSync","AlphabeticalList addSection for preview letter = " +mSections.get(i).mAlphabeticIndex);
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
		public View getView(int position, View convertView, ViewGroup parent)
		{
			if (convertView == null)
			{
				int sectionPosition =0;
				int browseIndex = 0;
				int sectionIndex = 0;

				for (int i=0; i< mSections.size(); i++)
				{
					AlphabeticalListViewSection sectionObj = mSections.get(i);
					browseIndex += sectionObj.itemsCount();
					sectionIndex = i;
					if ( browseIndex > position )
					{
						Log.e("@@MoSync","getView found the position index , on i= " + i);
						return sectionObj.getItem(position-sectionPosition).getRootView();
					}
					sectionPosition += sectionObj.itemsCount();
				}
				return mSections.get(sectionIndex).getItem(position-sectionPosition).getRootView();
			}

			return convertView;
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
			Log.e("getPositionForSection !!!!!!!! ", " section =  " + section);
			int browseIndex = 0;
			for (int i=0; i < section; i++)
			{
				browseIndex += mSections.get(i).itemsCount();
			}

			Log.e("getPositionForSection", "section = " + section + ",position = " + browseIndex);
			return browseIndex;
		}

		/**
		 * @see SectionIndexer.getSections.
		 */
		@Override
		public int getSectionForPosition(int position) {
			Log.e("getSectionForPosition!!!!!!!!! ", " called ");
			return 0;
		}

		/**
		 * @see SectionIndexer.getSections.
		 */
		@Override
		public Object[] getSections() {
			// The list view will call toString() on each object
			// to get the preview letter.
			Log.e("@@Mosync","AlphabeticalLsit getSections !");
			Log.e("@@Mosync","AlphabeticalLsit getSections, sections count = " + mSections.size());
			return mPreviewLetters;
		}

		/**
		 * @see BaseAdapter.getItem.
		 */
		@Override
		public Object getItem(int position) {
			Log.e("@@MoSync","AlphabeticalList  getItem, position = " + position);
			return getView(position, null,null);
		}

		/**
		 * @see BaseAdapter.getItemId.
		 */
		@Override
		public long getItemId(int position)
		{
			Log.e("@@MoSync","AlphabeticalList getitemId, position = " + position);
			return position;
		}
	}

}