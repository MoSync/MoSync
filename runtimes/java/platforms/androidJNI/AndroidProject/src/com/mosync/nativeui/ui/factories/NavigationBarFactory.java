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

import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.ui.widgets.NavigationBarWidget;

import android.app.Activity;
import android.view.Gravity;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

/**
 * Creates a horizontal linear layout with a button and a label.
 *
 * @author emma
 */
public class NavigationBarFactory implements AbstractViewFactory
{
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
		LinearLayout layout = new LinearLayout( activity );
		layout.setGravity( Gravity.CENTER_VERTICAL );
//		layout.setGravity( Gravity.CENTER_HORIZONTAL );
		layout.setOrientation( LinearLayout.HORIZONTAL );

		ImageView titleImage = new ImageView( activity );
		titleImage.setPadding(5, 5, 5, 5);
		titleImage.setVisibility(View.GONE);
		layout.addView(titleImage);

		TextView titleLabel = new TextView( activity );
		titleLabel.setVisibility(View.GONE);
		titleLabel.setPadding(5, 5, 5, 5);
		layout.addView(titleLabel);

		return new NavigationBarWidget( handle, layout);
	}

}