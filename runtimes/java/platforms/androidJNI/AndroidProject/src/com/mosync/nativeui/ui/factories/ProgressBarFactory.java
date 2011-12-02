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

import android.app.Activity;
import android.widget.ProgressBar;

import com.mosync.nativeui.ui.widgets.ProgressBarWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A factory that creates progress bar and sets up the default
 * behavior for sending events when clicked.
 *
 * @author emma
 */
public class ProgressBarFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		ProgressBar progressBar = new ProgressBar( activity, null, android.R.attr.progressBarStyleHorizontal );

		return new ProgressBarWidget( handle, progressBar );
	}
}
