package com.mosync.nativeui.ui.factories;

import com.mosync.nativeui.ui.widgets.Widget;

import android.content.Context;
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
	public Widget create(Context context, int handle) 
	{
		LinearLayout layout = new LinearLayout( context );
		layout.setOrientation( LinearLayout.HORIZONTAL );

		return new com.mosync.nativeui.ui.widgets.LinearLayout( handle, layout );
	}

}
