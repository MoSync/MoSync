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
