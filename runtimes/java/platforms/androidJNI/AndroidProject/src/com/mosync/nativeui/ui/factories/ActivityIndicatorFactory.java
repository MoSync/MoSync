package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.ProgressBar;

import com.mosync.nativeui.ui.widgets.ActivityIndicatorWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A factory that creates activity indicator and sets up the default
 * behavior for sending events when clicked.
 *
 * @author Emma
 */
public class ActivityIndicatorFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		ProgressBar activityIndicator = new ProgressBar( activity );

		return new ActivityIndicatorWidget( handle, activityIndicator );
	}
}
