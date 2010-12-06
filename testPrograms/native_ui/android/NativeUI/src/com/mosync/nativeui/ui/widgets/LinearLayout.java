package com.mosync.nativeui.ui.widgets;

import android.view.ViewGroup;

public class LinearLayout extends Layout
{

	public LinearLayout(int handle, android.widget.LinearLayout linearLayout)
	{
		super(handle, linearLayout);
	}

	/**
	 * @see Layout.updateLayoutParams.
	 */
	@Override
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new android.widget.LinearLayout.LayoutParams( mosyncLayoutParams.getWidth( ) , mosyncLayoutParams.getHeight( ) );
	}
}
