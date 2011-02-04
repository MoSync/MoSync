package com.mosync.nativeui.ui.widgets;

import android.view.Gravity;
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
		android.widget.FrameLayout.LayoutParams nativeLayoutParams
			= new android.widget.FrameLayout.LayoutParams( mosyncLayoutParams.getWidth( ) , mosyncLayoutParams.getHeight( ) );
		nativeLayoutParams.gravity = Gravity.LEFT | Gravity.TOP;

		return nativeLayoutParams;
	}

}
