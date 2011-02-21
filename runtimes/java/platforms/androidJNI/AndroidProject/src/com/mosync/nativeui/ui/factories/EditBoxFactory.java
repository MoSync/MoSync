package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.widget.EditText;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.EditBoxWidget;
import com.mosync.nativeui.ui.widgets.Widget;

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
		EditText editBox = new EditText( activity );
		editBox.setOnKeyListener( new OnKeyListener( ) {
			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event)
			{
				if( (event.getAction( ) == KeyEvent.ACTION_DOWN)
						&& (keyCode == KeyEvent.KEYCODE_ENTER) )
				{
					EventQueue.getDefault( ).postWidgetEvent( IX_WIDGET.WIDGET_EVENT_CLICKED, handle );
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
