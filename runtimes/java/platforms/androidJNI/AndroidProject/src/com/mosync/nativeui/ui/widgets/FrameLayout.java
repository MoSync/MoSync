package com.mosync.nativeui.ui.widgets;

import android.view.ViewGroup;

public class FrameLayout extends Layout
{

	public FrameLayout(int handle, android.widget.FrameLayout view)
	{
		super( handle, view );
	}
	
	/**
	 * @see Layout.updateLayoutParams.
	 */
	@Override
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new android.widget.FrameLayout.LayoutParams( mosyncLayoutParams.getWidth( ) , mosyncLayoutParams.getHeight( ) );
	}
}
