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
import android.content.Context;
import android.view.View;
import android.widget.ListView;

import com.mosync.nativeui.ui.widgets.ListLayout;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a list capable of displaying a set of widgets.
 *
 * @author fmattias
 */
public class ListFactory implements AbstractViewFactory
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

		return new ListLayout( handle, listView );
	}

	/**
	 * Nullify the addView and removeView methods in ListView
	 * so that it can be used in a convenient way as a Layout
	 * subclass.
	 *
	 * @author fmattias
	 */
	public static class CustomListView extends ListView
	{
		public CustomListView(Context context)
		{
			super( context );
		}

		@Override
		public void addView(View child, int index)
		{
		}

		@Override
		public void addView(View child)
		{
		}

		@Override
		public void removeView(View child)
		{
		}

		@Override
		public void removeViewAt(int index)
		{
		}
	}
}
