/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
