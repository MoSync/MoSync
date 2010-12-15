package com.mosync.nativeui.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import com.mosync.nativeui.util.LayoutParamsSetter;

import android.util.Log;
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
	/**
	 * Feeds the list with views.
	 */
	ViewAdapter m_viewAdapter = new ViewAdapter( );
	
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
		
		child.getView( ).setFocusable( false );
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
	}
}
