package com.mosync.nativeui.ui.factories;

import com.mosync.nativeui.ui.widgets.Widget;

import android.app.Activity;

/**
 * Interface for the factories that creates a widget. To create
 * a new factory do the following:
 * 
 * 1. Implement the AbstractViewFactory interface.
 * 2. Associate the factory with the type of widgets it creates in 
 *    ViewFactory.
 * 
 * @author fmattias
 */
public interface AbstractViewFactory
{	
	/**
	 * Creates a view and returns it.
	 * @param activity The activity within which the widget is created.
	 * @param handle Integer representing the handle of the widget.
	 * 
	 * @return A view associated with this factory, or null if it
	 *         couldn't be created.
	 */
	public abstract Widget create(Activity activity, int handle);
}
