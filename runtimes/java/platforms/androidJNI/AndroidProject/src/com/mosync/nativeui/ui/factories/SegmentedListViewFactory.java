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

import com.mosync.nativeui.ui.widgets.SegmentedListLayout;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.ui.factories.ListFactory.CustomListView;

/**
 * A factory that crates a segmented custom list view.
 * The behaviour for sending events is handled by the SegmentedListLayout
 * widget.
 *
 * @author emma
 */
public class SegmentedListViewFactory implements AbstractViewFactory
{
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
		ListView listView = new CustomListView( activity );

		// Use the cache color hint optimization.
		// This makes the background not to turn black while scrolling the list.
		listView.setCacheColorHint(0x00000000);

		return new SegmentedListLayout( handle, activity, listView );
	}

}