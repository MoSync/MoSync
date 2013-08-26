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
		LayoutParams layoutParams = new LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT );
		layout.setLayoutParams( layoutParams );

		return new StackScreenWidget( handle, layout );
	}
}
