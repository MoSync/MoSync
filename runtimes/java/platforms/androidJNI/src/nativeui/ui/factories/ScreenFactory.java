package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.FrameLayout;

import com.mosync.nativeui.ui.widgets.ScreenWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Responsible for creating screens.
 * 
 * @author fmattias
 */
public class ScreenFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, int handle)
	{
		FrameLayout layout = new FrameLayout( activity );
		
		return new ScreenWidget( handle, layout );
	}
	
}