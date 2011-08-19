package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.ProgressBar;

import com.mosync.nativeui.ui.widgets.ProgressBarWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A factory that creates progress bar and sets up the default
 * behavior for sending events when clicked.
 *
 * @author emma
 */
public class ProgressBarFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		ProgressBar progressBar = new ProgressBar( activity, null, android.R.attr.progressBarStyleHorizontal );

		return new ProgressBarWidget( handle, progressBar );
	}
}
