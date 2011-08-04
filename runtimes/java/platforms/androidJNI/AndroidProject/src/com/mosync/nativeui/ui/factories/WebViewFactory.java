package com.mosync.nativeui.ui.factories;

import android.app.Activity;

import com.mosync.nativeui.ui.widgets.WebWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a web view which sends MoSync events for some of its
 * state changes.
 * 
 * @author Mikael Kindborg
 */
public class WebViewFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		return WebWidget.create(activity, handle);
	}
}
//End of class WebViewFactory.
