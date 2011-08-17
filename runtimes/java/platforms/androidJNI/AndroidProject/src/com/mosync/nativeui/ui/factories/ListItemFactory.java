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
