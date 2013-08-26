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
import android.app.AlertDialog;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;

import com.mosync.nativeui.ui.widgets.DialogWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Responsible for creating dialogs.
 *
 * @author emma
 */
public class DialogFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, int handle)
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(activity);

		FrameLayout layout = new FrameLayout( activity );
		// Layout fills the entire dialog.
		LayoutParams layoutParams = new LayoutParams( ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT );
		layout.setLayoutParams( layoutParams );

		builder.setView(layout);

		// The new widget is basically a dialog that contains an empty layout.
		return new DialogWidget( handle, builder, layout);
	}
}