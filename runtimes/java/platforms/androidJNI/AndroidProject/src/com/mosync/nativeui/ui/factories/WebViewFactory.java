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

import com.mosync.nativeui.ui.widgets.WebWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a web view which sends MoSync events for some of its
 * state changes.
 *
 * @author Mikael Kindborg
 */
public class WebViewFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, int handle)
	{
		return WebWidget.create(activity, handle);
	}
}
