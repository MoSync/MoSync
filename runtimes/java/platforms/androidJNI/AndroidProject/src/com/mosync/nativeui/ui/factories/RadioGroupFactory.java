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
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.RadioGroupWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a radio group widget, that acts like a container for radio buttons.
 * Radio buttons are grouped inside RadioGroup element so that no more than
 * one can be selected at a time.
 *
 * @author emma
 */
public class RadioGroupFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, final int handle)
	{
		RadioGroup radioGroup = new RadioGroup( activity );
		radioGroup.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(RadioGroup group, int checkedId) {
				EventQueue.getDefault( ).postRadioGroupItemSelected(handle, checkedId);
			}
		});

		return new RadioGroupWidget( handle, radioGroup);
		}
}
