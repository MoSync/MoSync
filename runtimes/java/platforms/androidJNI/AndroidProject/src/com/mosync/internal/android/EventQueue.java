package com.mosync.internal.android;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_WIDGET;

/**
 * Class that handles posting of events to the MoSync
 * event queue.
 * 
 * TODO: Move posting of other event types here.
 */
public class EventQueue
{
	public static MoSyncThread sMoSyncThread;
	
	private static EventQueue DEFAULT = new EventQueue( );	
	
	public void postWidgetEvent(int widgetEventType, int widgetHandle)
	{
		postWidgetEvent( widgetEventType, widgetHandle, 0, 0 );
	}
	
	/**
	 * Post a widget event.
	 * 
	 * @param widgetEventType The type of the widget event.
	 * @param widgetHandle The handle of the widget.
	 * @param messageId The message id (may be zero and may also be used
	 * for params listItemIndex and searchBarButton, see MAEvent for
	 * documentation of these fields.
	 * @param messageSize The size of the message (may be zero).
	 */
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

