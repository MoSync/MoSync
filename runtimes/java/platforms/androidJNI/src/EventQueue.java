package com.mosync.internal.android;

import android.util.Log;

public class EventQueue
{
	private static EventQueue DEFAULT = new EventQueue();
	
	public static int CLICKED = 5;
	public static int ITEM_CLICKED = 6;
	
	public void postWidgetEvent(int widgetEventType, int widgetHandle)
	{
		postWidgetEvent( widgetEventType, widgetHandle, 0, 0 );
	}
	
	public void postWidgetEvent(int widgetEventType, int widgetHandle, int messageId, int messageSize)
	{
		int event[] = new int[ 5 ];
		
		event[0] = 26;
		event[1] = widgetEventType;
		event[2] = widgetHandle;
		event[3] = messageId;
		event[4] = messageSize;
		
		for(int i = 0; i < event.length; i++)
		{
			Log.i( "MoSync", "event[" + i + "] = " + event[ i ] );
		}
	}
	
	public static EventQueue getDefault()
	{
		return DEFAULT;
	}
}