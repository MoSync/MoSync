package com.mosync.nativeui.ui.widgets;

import android.view.ViewGroup;

/**
 * A class for a linear layout that is either horizontal
 * or vertical.
 * 
 * @author fmattias
 */
public class LinearLayout extends Layout
{

	/**
	 * Constructor.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param linearLayout A linear layout wrapped by this widget.
	 */
	public LinearLayout(int handle, android.widget.LinearLayout linearLayout)
	{
		super( handle, linearLayout );
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
