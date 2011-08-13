/* Copyright (C) 2011 MoSync AB

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

import android.content.Context;
import android.widget.TextView;

import com.mosync.nativeui.ui.widgets.LabelWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a label.
 * 
 * @author fmattias
 */
public class LabelFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Context context, int handle)
	{
		TextView textView = new TextView( context );
		return new LabelWidget( handle, textView );
	}
}
