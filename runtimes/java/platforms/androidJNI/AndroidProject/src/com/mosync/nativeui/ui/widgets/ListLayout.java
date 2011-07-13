package com.mosync.nativeui.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.BaseAdapter;
import android.widget.ListView;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

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
	
	@Override
	public void addChildAt(Widget child, int index)
	{
		super.addChildAt( child, index );
		
		int listIndex = index;
		if( index == -1 )
		{
			listIndex = m_viewAdapter.getCount( );
		}
		
		child.getView( ).setFocusable( false );
		m_viewAdapter.addAt( child.getRootView( ), listIndex );
	}

	/**
	 * Add the child to the view adapter rather to
	 * the list view itself.
	 * 
	 * @see Layout.addChild.
	 */
	@Override
	public void removeChild(Widget child)
	{
		super.removeChild( child );

		m_viewAdapter.remove( child.getRootView( ) );
	}
	
	/**
	 * @see Layout.updateLayoutParams.
	 */
	@Override
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new AbsListView.LayoutParams( mosyncLayoutParams.getWidth( ) , mosyncLayoutParams.getHeight( ) );
	}
	
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}
		
		if( property.equals( Types.WIDGET_PROPERTY_REVERSED ) )
		{
			m_viewAdapter.setReversed( BooleanConverter.convert( value ) );
		}
		else
		{
			return false;
		}
		
		return true;
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
	}
}
