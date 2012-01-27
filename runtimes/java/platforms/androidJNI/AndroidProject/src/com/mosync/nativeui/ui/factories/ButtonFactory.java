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
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;
import android.widget.Button;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.ui.widgets.ButtonWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.MoSyncSendOnClick;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A factory that creates button and sets up the default
 * behavior for sending events when clicked.
 *
 * @author fmattias
 */
public class ButtonFactory implements AbstractViewFactory
{
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, final int handle)
	{
		Button b = new Button( activity );
		b.setOnClickListener( new MoSyncSendOnClick( handle ) );

		b.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
		        if(event.getAction() == MotionEvent.ACTION_DOWN) {
					EventQueue.getDefault( ).postWidgetEvent( IX_WIDGET.MAW_EVENT_POINTER_PRESSED, handle );
		        } else if (event.getAction() == MotionEvent.ACTION_UP) {
					EventQueue.getDefault( ).postWidgetEvent( IX_WIDGET.MAW_EVENT_POINTER_RELEASED, handle );
		        }
		        // The event is not consumed yet, onClick can be received.
		        return false;
			}
		});

		b.setOnKeyListener(new OnKeyListener() {
			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event) {
				if (event.getAction() == KeyEvent.ACTION_DOWN) {
					EventQueue.getDefault( ).postWidgetEvent( IX_WIDGET.MAW_EVENT_POINTER_PRESSED, handle );
				} else if (event.getAction() == KeyEvent.ACTION_UP) {
					EventQueue.getDefault( ).postWidgetEvent( IX_WIDGET.MAW_EVENT_POINTER_RELEASED, handle );
				}
				return false;
			}
		});

		ButtonWidget button = new ButtonWidget( handle, b );

		try
		{
			button.setProperty( Types.WIDGET_PROPERTY_TEXT_HORIZONTAL_ALIGNMENT, Types.WIDGET_HORIZONTAL_CENTER );
			button.setProperty( Types.WIDGET_PROPERTY_TEXT_VERTICAL_ALIGNMENT, Types.WIDGET_VERTICAL_CENTER );
		}
		catch(PropertyConversionException pe)
		{
			// If this happens, there is a bug in the implementation, just return null.
			return null;
		}

		return button;
	}
}
