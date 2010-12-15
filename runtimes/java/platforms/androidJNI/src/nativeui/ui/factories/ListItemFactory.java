package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.mosync.java.android.R;
import com.mosync.nativeui.ui.widgets.ListItemWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a list item.
 * 
 * @author fmattias
 */
public class ListItemFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		ViewGroup layout = (ViewGroup) activity.getLayoutInflater( ).inflate( R.layout.list_item, null );
		
		TextView label = (TextView) layout.findViewById( R.id.list_item_text );
		ImageView icon = (ImageView) layout.findViewById( R.id.list_item_icon );
		
		return new ListItemWidget( handle, layout, label, icon );
	}
}
