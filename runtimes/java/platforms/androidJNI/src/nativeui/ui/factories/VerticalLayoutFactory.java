package com.mosync.nativeui.ui.factories;

import com.mosync.nativeui.ui.widgets.Widget;

import android.app.Activity;
import android.widget.LinearLayout;

/**
 * Creates a vertical linear layout.
 * 
 * @author fmattias
 */
public class VerticalLayoutFactory implements AbstractViewFactory
{
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle) 
	{
		LinearLayout layout = new LinearLayout( activity );
		layout.setOrientation( LinearLayout.VERTICAL );

		return new com.mosync.nativeui.ui.widgets.LinearLayout( handle, layout );
	}

}
