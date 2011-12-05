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

import android.app.Activity;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.widget.EditText;
import android.widget.TextView;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.EditBoxWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.KeyboardManager;

/**
 * A factory that creates an editbox that sends a click event
 * when the enter key is pressed.
 *
 * @author fmattias
 */
public class EditBoxFactory implements AbstractViewFactory
{
	@Override
	public Widget create(Activity activity, final int handle)
	{
		final EditText editBox = new EditText( activity );
		editBox.setOnKeyListener( new OnKeyListener( ) {
			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event)
			{
				EventQueue.getDefault().postWidgetEvent(IX_WIDGET.MAW_EVENT_EDIT_BOX_TEXT_CHANGED, handle);

				if( (event.getAction( ) == KeyEvent.ACTION_DOWN)
						&& (keyCode == KeyEvent.KEYCODE_ENTER) )
				{
					EventQueue.getDefault( ).postWidgetEvent( IX_WIDGET.MAW_EVENT_EDIT_BOX_RETURN, handle );
					return true;
				}
				else
				{
					return false;
				}
			}
		});

		editBox.setSingleLine( );

		return new EditBoxWidget( handle, editBox );
	}
}
