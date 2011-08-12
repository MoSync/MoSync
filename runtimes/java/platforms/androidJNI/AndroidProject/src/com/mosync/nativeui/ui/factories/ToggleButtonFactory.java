package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.ToggleButton;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.ToggleButtonWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a toggle button widget, that sends events once it's
 * state changes.
 * 
 * @author emma tresanszki 
 * 
 */
public class ToggleButtonFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, final int handle)
	{
		ToggleButton toggleBtn = new ToggleButton(activity);
		toggleBtn.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
			{
				EventQueue.getDefault( ).postWidgetClickedEvent( handle, isChecked );
				
			}
		});
		
		return new ToggleButtonWidget( handle, toggleBtn );
	}
	
}
