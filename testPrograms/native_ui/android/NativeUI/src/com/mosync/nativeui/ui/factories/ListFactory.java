package com.mosync.nativeui.ui.factories;

import com.mosync.nativeui.ui.widgets.ListBox;
import com.mosync.nativeui.ui.widgets.Widget;

import android.content.Context;
import android.widget.ListView;

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
	public Widget create(Context context, int handle)
	{
		ListView listView = new ListView( context );
		return new ListBox( handle, listView );
	}
}
