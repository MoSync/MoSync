/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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