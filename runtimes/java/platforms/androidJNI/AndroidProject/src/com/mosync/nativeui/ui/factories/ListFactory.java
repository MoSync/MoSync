package com.mosync.nativeui.ui.factories;

import static com.mosync.internal.generated.IX_WIDGET.WIDGET_EVENT_ITEM_CLICKED;

import android.app.Activity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;

import com.mosync.nativeui.ui.widgets.ListLayout;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.internal.android.EventQueue;

/**
 * Creates a list capable of displaying a set of widgets.
 * 
 * @author fmattias
 */
public class ListFactory implements AbstractViewFactory
{	
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
		ListView listView = new ListView( activity );
		
		listView.setOnItemClickListener( new ListOnItemClickListener( ) );
		return new ListLayout( handle, listView );
	}
	
	/**
	 * Class responsible for sending a mosync event when an item
	 * in the list has been clicked.
	 * 
	 * @author fmattias
	 */
	public class ListOnItemClickListener implements OnItemClickListener
	{
		/**
		 * @see OnItemClickListener.onItemClick.
		 */
		@Override
		public void onItemClick(AdapterView<?> parent, View view, int position, long id)
		{
			// Assume that the view's id is always the same as the handle.
			EventQueue.getDefault( ).postWidgetEvent( WIDGET_EVENT_ITEM_CLICKED, view.getId( ) );
		}	
	}
}
