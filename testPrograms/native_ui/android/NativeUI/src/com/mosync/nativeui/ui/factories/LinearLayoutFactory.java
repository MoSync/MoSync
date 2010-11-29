package com.mosync.nativeui.ui.factories;

import com.mosync.nativeui.ui.widgets.Layout;
import com.mosync.nativeui.ui.widgets.Widget;

import android.content.Context;
import android.widget.LinearLayout;

/**
 * Creates a vertical linear layout.
 * 
 * @author fmattias
 */
public class LinearLayoutFactory implements AbstractViewFactory
{
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Context context, int handle) 
	{
		LinearLayout layout = new LinearLayout( context );
		layout.setOrientation( LinearLayout.VERTICAL );

		return new Layout( handle, layout );
	}

}
