/* Copyright (C) 2013 MoSync AB

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

import android.app.Activity;
import android.widget.Spinner;

import com.mosync.nativeui.ui.widgets.SpinnerWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a custom spinner which accepts any kind of layouts as children.
 *
 * @author emma tresanszki
 */
public class CustomPickerFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, final int handle)
	{
		Spinner spinner = new Spinner( activity );

		return new SpinnerWidget( activity, handle, spinner);
	}
}
