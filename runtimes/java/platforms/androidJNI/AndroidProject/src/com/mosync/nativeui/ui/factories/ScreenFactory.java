package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;

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
		
		// Screen fills the whole screen.
		LayoutParams layoutParams = new LayoutParams( ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT );
		layout.setLayoutParams( layoutParams );
		
		return new ScreenWidget( handle, layout );
	}	
}