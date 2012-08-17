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
import android.widget.ListView;

import com.mosync.nativeui.ui.widgets.AlphabeticalListViewSection;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a section for a custom type of list view: Alphabetical list view.
 * A section is in fact only a container, and all the basic widget properties
 * are not applicable for it.
 *
 * It's main characteristic is the alphabetic index String that is
 * displayed on the fast scroll bar.
 *
 * @author emma
 */
public class AlphabeticalListSectionFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		ListView listLayout = new ListView( activity );

		return new AlphabeticalListViewSection( handle, listLayout, activity);
	}
}