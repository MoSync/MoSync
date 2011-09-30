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
import android.app.AlertDialog;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;

import com.mosync.nativeui.ui.widgets.DialogWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Responsible for creating dialogs.
 *
 * @author emma
 */
public class DialogFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, int handle)
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(activity);

		FrameLayout layout = new FrameLayout( activity );
		// Layout fills the entire dialog.
		LayoutParams layoutParams = new LayoutParams( ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT );
		layout.setLayoutParams( layoutParams );

		builder.setView(layout);

		// The new widget is basically a dialog that contains an empty layout.
		return new DialogWidget( handle, builder, layout);
	}
}