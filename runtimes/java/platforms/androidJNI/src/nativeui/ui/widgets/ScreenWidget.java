package com.mosync.nativeui.ui.widgets;

import com.mosync.nativeui.util.LayoutParamsSetter;

import android.view.ViewGroup;

/**
 * A screen widget that represents the root of a widget
 * tree.
 * 
 * Note: For now a screen widget is just a wrapper to a view
 * like all other widgets, but by default it fills the whole
 * screen.
 * 
 * @author fmattias
 */
public class ScreenWidget extends Layout
{
	/**
	 * Constructor
	 * 
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A screen wrapped by this widget.
	 */
	public ScreenWidget(int handle, ViewGroup view)
	{
		super( handle, view );
		
		// Screen fills the whole screen.
		getLayoutParams( ).width = ViewGroup.LayoutParams.FILL_PARENT;
		getLayoutParams( ).height = ViewGroup.LayoutParams.FILL_PARENT;
		
		ViewGroup.LayoutParams nativeLayoutParams = createNativeLayoutParams( getLayoutParams( ) );
		LayoutParamsSetter.setPossibleParams( getLayoutParams( ), nativeLayoutParams );
		getView( ).setLayoutParams( nativeLayoutParams );
	}
}