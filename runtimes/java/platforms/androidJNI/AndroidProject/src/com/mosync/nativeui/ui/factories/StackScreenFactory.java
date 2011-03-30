package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.ViewFlipper;

import com.mosync.nativeui.ui.widgets.StackScreenWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a stack screen.
 * 
 * The stack screen uses a frame layout to contain
 * the other screens, instead of directly using the
 * current screens root view. I designed it this way
 * to be able to have a StackScreen inside a tab screen.
 * Since there is no simple way to change the content of
 * a tab after it has been added.
 * 
 * @author fmattias
 *
 */
public class StackScreenFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		FrameLayout layout = new ViewFlipper( activity );

		// Screen fills the whole screen.
		LayoutParams layoutParams = new LayoutParams( ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT );
		layout.setLayoutParams( layoutParams );
		
		return new StackScreenWidget( handle, layout );
	}
}
