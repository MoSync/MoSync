package com.mosync.nativeui.ui.factories;

import com.mosync.nativeui.ui.widgets.Widget;

import android.app.Activity;
import android.widget.LinearLayout;

/**
 * Creates a horizontal linear layout.
 * 
 * @author fmattias
 */
public class HorizontalLayoutFactory implements AbstractViewFactory
{
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle) 
	{
		LinearLayout layout = new LinearLayout( activity );
		layout.setOrientation( LinearLayout.HORIZONTAL );

		return new com.mosync.nativeui.ui.widgets.LinearLayout( handle, layout );
	}

}
