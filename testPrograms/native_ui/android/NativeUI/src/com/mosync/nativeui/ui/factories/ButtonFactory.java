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

import com.mosync.nativeui.ui.widgets.ButtonWidget;
import com.mosync.nativeui.ui.widgets.Widget;

import android.content.Context;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

/**
 * A factory that creates button and sets up the default
 * behavior for sending events when clicked.
 * 
 * @author fmattias
 */
public class ButtonFactory implements AbstractViewFactory
{	
	/**
	 * Listener for the buttons.
	 */
	private MoSyncSendOnClick m_onClickListener = new MoSyncSendOnClick( );
	
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Context context, int handle)
	{
		Button b = new Button( context );
		b.setOnClickListener( m_onClickListener );
		
		return new ButtonWidget(handle, b);
	}
	
	/**
	 * A class that sends an onclick event when the
	 * button is clicked.
	 * 
	 * @author fmattias
	 */
	class MoSyncSendOnClick implements OnClickListener
	{

		@Override
		public void onClick(View v)
		{
			// TODO Send onclikc event to mosync.
		}
	}
}
