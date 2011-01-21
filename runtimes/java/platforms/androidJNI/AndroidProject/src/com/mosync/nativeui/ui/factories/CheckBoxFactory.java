package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.CheckBoxWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a check box widget, that sends events once it's
 * state changes.
 * 
 * @author fmattias
 */
public class CheckBoxFactory implements AbstractViewFactory
{

	@Override
	public Widget create(Activity activity, final int handle)
	{
		CheckBox checkBox = new CheckBox( activity );
		checkBox.setOnCheckedChangeListener( new OnCheckedChangeListener( ) {
			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
			{
				EventQueue.getDefault( ).postWidgetClickedEvent( handle, isChecked );
			}
		});
		
		return new CheckBoxWidget( handle, checkBox );
	}
	
	
}
