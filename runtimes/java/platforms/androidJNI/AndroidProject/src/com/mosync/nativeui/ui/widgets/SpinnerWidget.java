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
import java.util.List;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A Spinner is a widget that displays one child at a time and lets the user pick among them.
 *
 * @author emma tresanszki
 */
public class SpinnerWidget extends Layout
{
	/**
	 * Custom spinner adapter for single choice list.
	 */
	private CustomSpinnerAdapter mAdapter = null;

	/**
	 * The application's context.
	 */
	private Context mContext;

	/**
	 * The spinner view items.
	 */
	List<View> mSpinnerItems = new ArrayList<View>( );

	/**
	 * The objects to represent in the spinner. Only used for setting
	 * the right number of elements at runtime.
	 */
	String []mStringItems = {};

	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param numberPicker A number picker wrapped by this widget.
	 */
	public SpinnerWidget(Context activity, int handle, Spinner spinner)
	{
		super( handle, spinner );
		mContext = activity;
		mAdapter = new CustomSpinnerAdapter(
						activity,
						android.R.layout.simple_spinner_dropdown_item,
						mStringItems);

		spinner.setAdapter(mAdapter);
	}

	/**
	 * Adds a child to this layout at the given position.
	 *
	 * @param child The child to be added.
	 * @param index The index where to add the child. The child will have
	 *        this index after it has been added, the index starts at 0. If
	 *        the index is -1 it will be added at the end.
	 */
	@Override
	public int addChildAt(Widget child, int index)
	{
		if ( !(child instanceof Layout) )
		{
			Log.e( "MoSync",
			"maWidgetInsertChild: CustomPicerWidget can only contain Layouts.");
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}

		// Make sure childen get destroyed when parent is destroyed.
		child.setParent(this);

		int listIndex = index;
		if (index == -1)
		{
			listIndex = mStringItems.length;
		}

		m_children.add( listIndex, child );

		setSpinnerAdapter();

		return IX_WIDGET.MAW_RES_OK;
	}

	/**
	 * Recreate the adapter that provides the data for the spinnner,
	 * and assign it to the spinner each time an item is added/removed.
	 */
	private void setSpinnerAdapter()
	{
		ArrayList<String> itemsList = new ArrayList<String>();
		for (int i=0; i < m_children.size(); i++)
		{
			itemsList.add("DEFAULT");
		}
		mStringItems = new String[itemsList.size()];

		// Convert to an array of objects.
		itemsList.toArray(mStringItems);

		// Recreate adapter for the right number of items.
		mAdapter = new CustomSpinnerAdapter(
						mContext,
						android.R.layout.simple_spinner_dropdown_item,
						mStringItems);
		((Spinner) getView()).setAdapter(mAdapter);
	}

	/**
	 * Removes a child form this layout.
	 *
	 * The child's parent will be set to null.
	 *
	 * @param child
	 */
	@Override
	public int removeChild(Widget child)
	{
		child.setParent( null );
		m_children.remove( child );

		setSpinnerAdapter();

		return IX_WIDGET.MAW_RES_OK;
	}

	/**
	 * Custom adapter for the single-choice spinner.
	 * @author emma
	 *
	 */
    public class CustomSpinnerAdapter extends ArrayAdapter<String>
    {

		public CustomSpinnerAdapter(Context context, int textViewResourceId,
				String[] objects) {
			super(context, textViewResourceId, objects);
		}

        @Override
        public View getDropDownView(int position, View convertView,ViewGroup parent) {
            return getCustomView(position, convertView, parent);
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            return getCustomView(position, convertView, parent);
        }

        public View getCustomView(int position, View convertView, ViewGroup parent) {
			if (convertView != null)
				return convertView;

			AbsListView.LayoutParams layoutParams = new AbsListView.LayoutParams(
							AbsListView.LayoutParams.FILL_PARENT, AbsListView.LayoutParams.FILL_PARENT);

			m_children.get(position).getView().setLayoutParams(layoutParams);
			return m_children.get(position).getView();
        }
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

		Spinner spinner = (Spinner) getView();

		return false;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		Spinner spinner = (Spinner) getView();


		return super.getProperty( property );

	}
}
