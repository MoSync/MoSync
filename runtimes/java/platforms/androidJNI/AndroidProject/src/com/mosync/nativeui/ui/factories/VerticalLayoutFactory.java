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

import android.app.Activity;
import android.widget.LinearLayout;

/**
 * Creates a vertical linear layout.
 * 
 * @author fmattias
 */
public class VerticalLayoutFactory implements AbstractViewFactory
{
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle) 
	{
		LinearLayout layout = new LinearLayout( activity );
		layout.setOrientation( LinearLayout.VERTICAL );

		return new com.mosync.nativeui.ui.widgets.LinearLayout( handle, layout );
	}

}
