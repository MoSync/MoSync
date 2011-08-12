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

import com.mosync.nativeui.util.LayoutParamsSetter;

import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.BaseAdapter;
import android.widget.ListView;

/**
 * This class wraps a list that displays a list of views.
 * The intention of this class is display views that all
 * has the same structure, otherwise a linear layout
 * may be preferred.
 * 
 * @author fmattias
 */
public class ListLayout extends Layout
{
	ViewAdapter m_viewAdapter = new ViewAdapter( );
	
	public ListLayout(int handle, ListView listView)
	{
		super(handle, listView);
		listView.setAdapter( m_viewAdapter );
	}

	/**
	 * Add the child to the view adapter rather to
	 * the list view itself.
	 * 
	 * @see Layout.addChild.
	 */
	@Override
	public void addChild(Widget child)
	{
		child.setParent( this );
		m_children.add( child );
		
		// Set layout params for the child
		ViewGroup.LayoutParams nativeLayoutParams = createNativeLayoutParams( child.getLayoutParams( ) );
		LayoutParamsSetter.setPossibleParams( child.getLayoutParams( ), nativeLayoutParams );
		child.getView( ).setLayoutParams( nativeLayoutParams );
		
		m_viewAdapter.add( child.getView( ) );
	}
	
	/**
	 * @see Layout.updateLayoutParams.
	 */
	@Override
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new AbsListView.LayoutParams( mosyncLayoutParams.getWidth( ) , mosyncLayoutParams.getHeight( ) );
	}
	
	/**
	 * An adapter that feeds the list view with views.
	 * 
	 * @author fmattias
	 */
	class ViewAdapter extends BaseAdapter
	{
		/**
		 * The list of views contained in this adapter.
		 */
		List<View> m_views = new ArrayList<View>();
		
		public ViewAdapter()
		{
			
		}
		
		/**
		 * Add a view to the adapter and notify the listeners.
		 * 
		 * @param view The view to add.
		 */
		public void add(View view)
		{
			if( view != null )
			{
				m_views.add( view );
				notifyDataSetChanged( );
			}
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
				return m_views.get( position );
			}
			else
			{
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
	}
}
