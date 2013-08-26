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
