package com.mosync.nativeui.ui.factories;

import android.content.Context;
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
	public Widget create(Context context, int handle)
	{
		TextView textView = new TextView( context );
		return new LabelWidget( handle, textView );
	}
}
