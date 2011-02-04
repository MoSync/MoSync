package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

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
		RelativeLayout listItemLayout = new RelativeLayout( activity );
		listItemLayout.setPadding( 5, 5, 5, 5 );
		
		TextView label = new TextView( activity );
		ImageView icon = new ImageView( activity );
		icon.setId( 100 );
		
		RelativeLayout.LayoutParams labelParams = new RelativeLayout.LayoutParams( RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT );
		labelParams.addRule( RelativeLayout.ALIGN_PARENT_RIGHT );
		labelParams.addRule( RelativeLayout.RIGHT_OF, icon.getId( ) );
		labelParams.addRule( RelativeLayout.CENTER_VERTICAL );
		labelParams.setMargins( 5, 0, 0, 5 );
		label.setLayoutParams( labelParams );
		
		label.setTextSize( 15.0f );
		
		RelativeLayout.LayoutParams iconParams = new RelativeLayout.LayoutParams( RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT );
		iconParams.addRule( RelativeLayout.ALIGN_PARENT_LEFT );
		iconParams.addRule( RelativeLayout.CENTER_VERTICAL );
		icon.setLayoutParams( iconParams );
		
		listItemLayout.addView( icon );
		listItemLayout.addView( label );
		
		return new ListItemWidget( handle, listItemLayout, label, icon );
	}
}
