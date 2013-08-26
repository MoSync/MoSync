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

import java.util.ArrayList;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A Spinner is a widget that displays one child at a time and lets the user pick among them.
 *
 * @author emma tresanszki
 */
public class SpinnerWidget extends Layout implements OnItemSelectedListener
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
	 * The objects to represent in the spinner. Only used for setting
	 * the right number of elements at runtime.
	 */
	private String []mStringItems = {};

	/**
	 * Store item's layout params. When those are set, all items will have the
	 * same dimensions.
	 * Note: when getting the MAW_CUSTOM_PICKER_ROW_HEIGHT or MAW_CUSTOM_PICKER_ROW_WIDTH
	 * properties before the items are drawn on the screen, FILL_PARENT will be returned
	 * instead of the actual values.
	 */
	int mItemHeight = AbsListView.LayoutParams.MATCH_PARENT;
	int mItemWidth = AbsListView.LayoutParams.MATCH_PARENT;

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
		// A spinner does not support item click events, just selection.
		spinner.setOnItemSelectedListener(this);
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

			AbsListView.LayoutParams layoutParams =
							new AbsListView.LayoutParams(mItemWidth, mItemHeight);
			m_children.get(position).getView().setLayoutParams(layoutParams);

			return m_children.get(position).getView();
        }
   }

	/**
	 * Updates the Android layout params for the given child, according
	 * to the parameters specified in the child. Only the parameters that
	 * are supported for the layout will be taken into consideration.
	 *
	 * @param child The child for which to update the layout params.
	 */
    @Override
	public void updateLayoutParamsForChild(Widget child)
	{
		ViewGroup.LayoutParams nativeLayoutParams =
				createNativeLayoutParams( child.getLayoutParams( ) );

		AbsListView.LayoutParams layoutParams =
				new AbsListView.LayoutParams(nativeLayoutParams.width, nativeLayoutParams.height);

		child.getView().setLayoutParams(layoutParams);
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
		if( property.equals(IX_WIDGET.MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX) )
		{
			int selectIndex = IntConverter.convertPositiveNumber(value);
			if ( selectIndex > m_children.size()-1)
				throw new InvalidPropertyValueException(property, value);
			spinner.setSelection( selectIndex );
		}
		else if( property.equals(IX_WIDGET.MAW_CUSTOM_PICKER_ROW_HEIGHT) )
		{
			mItemHeight = IntConverter.convert( value );

			// Set height of all children widgets.
			for (int i=0 ; i < m_children.size(); i++)
			{
				Widget child = m_children.get(i);

				ViewGroup.LayoutParams nativeLayoutParams =
					createNativeLayoutParams( child.getLayoutParams( ) );

				AbsListView.LayoutParams layoutParams =
						new AbsListView.LayoutParams(nativeLayoutParams.width, mItemHeight);

				child.getView().setLayoutParams(layoutParams);
			}
		}
		else if( property.equals(IX_WIDGET.MAW_CUSTOM_PICKER_ROW_WIDTH) )
		{
			mItemWidth = IntConverter.convert( value );

			for (int i=0 ; i < m_children.size(); i++)
			{
				Widget child = m_children.get(i);

				ViewGroup.LayoutParams nativeLayoutParams =
					createNativeLayoutParams( child.getLayoutParams( ) );

				AbsListView.LayoutParams layoutParams =
						new AbsListView.LayoutParams(mItemWidth, nativeLayoutParams.height);

				child.getView().setLayoutParams(layoutParams);
			}
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
		Spinner spinner = (Spinner) getView();
		if( property.equals(IX_WIDGET.MAW_CUSTOM_PICKER_SELECTED_ITEM_INDEX) )
		{
			return String.valueOf( spinner.getSelectedItemPosition() );
		}
		else if( property.equals(IX_WIDGET.MAW_CUSTOM_PICKER_ROW_HEIGHT) )
		{
			return String.valueOf( mItemHeight );
		}
		else if( property.equals(IX_WIDGET.MAW_CUSTOM_PICKER_ROW_WIDTH) )
		{
			return String.valueOf( mItemWidth );
		}
		else
		{
			return super.getProperty( property );
		}
	}

	@Override
	public void onItemSelected(AdapterView<?> arg0, View view,
			int position, long id) {
		// Post event to the MoSync queue.
		EventQueue.getDefault( ).postCustomPickerItemSelectedEvent(
						getHandle(), position );
	}

	@Override
	public void onNothingSelected(AdapterView<?> arg0) {
	}
}
