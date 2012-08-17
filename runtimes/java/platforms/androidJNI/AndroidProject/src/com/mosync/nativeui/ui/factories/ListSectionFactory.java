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

import com.mosync.nativeui.ui.widgets.ListViewSection;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A section is in fact only a container, and all the basic widget properties
 * are not applicable for it.
 * Creates a section for two custom types of list view:
 * 	- Segmented list view. It reproduces the iOS appearance:
 * 		- customizable header list item.
 * 		- section items with list view items, and
 * 		- customizable footer list item.
 * 		- Header and footer items cannot be selected.
 * 		- The appearance: font size, font color, background color
 * 		should be set according to the device theme.
 * 		- If no items are added, the implicit header and footer are still shown.
 *
 *  - Alphabetical list view with preview letter setting.
 *
 * @author emma
 */
public class ListSectionFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		ListView listLayout = new ListView( activity );

		return new ListViewSection( handle, listLayout, activity);
	}
}