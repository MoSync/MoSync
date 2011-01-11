package com.mosync.internal.android;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_WIDGET;

public class EventQueue
{
	public static MoSyncThread sMoSyncThread;
	
	private static EventQueue DEFAULT = new EventQueue( );	
	
	public void postWidgetEvent(int widgetEventType, int widgetHandle)
	{
		postWidgetEvent( widgetEventType, widgetHandle, 0, 0 );
	}
	
	public void postWidgetEvent(int widgetEventType, int widgetHandle, int messageId, int messageSize)
	{
		int event[] = new int[ 5 ];
		
		event[0] = EVENT_TYPE_WIDGET;
		event[1] = widgetEventType;
		event[2] = widgetHandle;
		event[3] = messageId;
		event[4] = messageSize;
		
		sMoSyncThread.postEvent( event );
	}

	public static EventQueue getDefault()
	{
		return DEFAULT;
	}
}

