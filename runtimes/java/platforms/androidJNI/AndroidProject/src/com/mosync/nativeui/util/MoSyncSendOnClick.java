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