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