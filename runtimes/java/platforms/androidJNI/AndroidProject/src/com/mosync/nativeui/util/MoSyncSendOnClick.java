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

package com.mosync.nativeui.util;

import android.view.View;
import android.view.View.OnClickListener;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;

/**
 * A class responsible for sending mosync widget events
 * when a button is clicked.
 * 
 * @author fmattias
 */
public class MoSyncSendOnClick implements OnClickListener
{
	private int m_handle = -1;
	
	/**
	 * Constructor.
	 * 
	 * @param handle The handle of the widget bound to this 
	 *               onclick handler.
	 */
	public MoSyncSendOnClick(int handle)
	{
		m_handle = handle;
	}

	/**
	 * @see OnClickListener.onClick.
	 */
	@Override
	public void onClick(View v)
	{
		EventQueue eventQueue = EventQueue.getDefault( );
		eventQueue.postWidgetEvent( IX_WIDGET.MAW_EVENT_CLICKED, m_handle );
	}
}