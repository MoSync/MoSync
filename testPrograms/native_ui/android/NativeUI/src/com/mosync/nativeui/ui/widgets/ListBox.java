package com.mosync.nativeui.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import android.view.View;
import android.view.ViewGroup;
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
public class ListBox extends Layout
{
	ViewAdapter m_viewAdapter = new ViewAdapter( );
	
	public ListBox(int handle, ListView listView)
	{
		super(handle, listView);
		listView.setAdapter( m_viewAdapter );
	}

	/**
	 * Add the child to the view adapter rather to
	 * the list view itself.
	 */
	@Override
	public void addChild(Widget child)
	{
		child.setParent( this );
		m_children.add( child );
		m_viewAdapter.add( child.getView( ) );
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
			if( convertView == null )
			{
				if( position < m_views.size( ) )
				{
					return m_views.get( position );
				}
				else
				{
					return null;
				}
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
