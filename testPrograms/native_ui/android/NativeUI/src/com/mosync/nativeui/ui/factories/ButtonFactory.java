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
