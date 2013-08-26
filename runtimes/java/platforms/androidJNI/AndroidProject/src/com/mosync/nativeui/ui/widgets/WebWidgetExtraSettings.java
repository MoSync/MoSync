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

package com.mosync.nativeui.ui.widgets;

import android.webkit.WebView;

/**
 * WebView settings for versions of Android that may fail on the
 * lowest Android API level supported by MoSync.
 * @author Mikael Kindborg
 */
public class WebWidgetExtraSettings
{
	void makeSettings(WebView webView)
	{
		webView.getSettings().setAllowUniversalAccessFromFileURLs(true);
	}
}
