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
