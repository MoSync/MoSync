/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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