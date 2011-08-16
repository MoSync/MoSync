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
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;
import android.widget.ViewFlipper;

import com.mosync.nativeui.ui.widgets.StackScreenWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a stack screen.
 * 
 * The stack screen uses a frame layout to contain
 * the other screens, instead of directly using the
 * current screens root view. I designed it this way
 * to be able to have a StackScreen inside a tab screen.
 * Since there is no simple way to change the content of
 * a tab after it has been added.
 * 
 * @author fmattias
 *
 */
public class StackScreenFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		FrameLayout layout = new ViewFlipper( activity );

		// Screen fills the whole screen.
		LayoutParams layoutParams = new LayoutParams( ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT );
		layout.setLayoutParams( layoutParams );
		
		return new StackScreenWidget( handle, layout );
	}
}
