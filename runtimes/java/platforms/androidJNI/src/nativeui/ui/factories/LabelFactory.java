package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.TextView;

import com.mosync.nativeui.ui.widgets.LabelWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a label.
 * 
 * @author fmattias
 */
public class LabelFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		TextView textView = new TextView( activity );
		return new LabelWidget( handle, textView );
	}
}
