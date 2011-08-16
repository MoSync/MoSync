/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

package com.mosync.nativeui.ui.factories;

import com.mosync.nativeui.ui.widgets.Widget;

import android.content.Context;

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
	 * @param context The context within which the widget is created.
	 * @param handle Integer representing the handle of the widget.
	 * 
	 * @return A view associated with this factory, or null if it
	 *         couldn't be created.
	 */
	public abstract Widget create(Context context, int handle);
}
