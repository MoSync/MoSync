/* Copyright (C) 2012 MoSync AB

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
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.RadioButton;
import android.widget.RadioGroup;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.RadioButtonWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a radio button widget, that sends events once it's
 * state changes.
 * Radio buttons are automatically grouped inside RadioGroup element
 * so that no more than one can be selected at a time.
 *
 * @author emma
 */
public class RadioButtonFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, final int handle)
	{
		RadioButton radioButton = new RadioButton( activity );
		radioButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {

//				EventQueue.getDefault( ).postWidgetClickedEvent( handle, isChecked );

			}
		});
		return new RadioButtonWidget( handle, radioButton);
		}
}
