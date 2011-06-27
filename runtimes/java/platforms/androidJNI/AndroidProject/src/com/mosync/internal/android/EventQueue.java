package com.mosync.internal.android;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_WIDGET;

import com.mosync.internal.generated.IX_WIDGET;

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

	/**
	 * Post a widget event.
	 * 
	 * @param widgetEventType The type of the widget event.
	 * @param widgetHandle The handle of the widget.
	 * @param auxParam1 Parameter used by some (used by some 
	 *  messages for parameters like messageDataHandle, listItemIndex, 
	 *  searchBarButton, etc. See struct MAWidgetEventData for 
	 *  documentation of these fields.
	 * @param auxParam2 Parameter used by some messages. 
	 *  Set to zero if not used.
	 */
	public void postWidgetEvent(
		int widgetEventType, 
		int widgetHandle, 
		int auxParam1, 
		int auxParam2)
	{
		int event[] = new int[5];
		
		event[0] = EVENT_TYPE_WIDGET;
		event[1] = widgetEventType;
		event[2] = widgetHandle;
		event[3] = auxParam1;
		event[4] = auxParam2;
		
		sMoSyncThread.postEvent(event);
	}
	
	/**
	 * Convenience method for posting a widget event with 
	 * two parameters.
	 * 
	 * @param widgetEventType The type of the widget event.
	 * @param widgetHandle The handle of the widget.
	 */
	public void postWidgetEvent(int widgetEventType, int widgetHandle)
	{
		postWidgetEvent(widgetEventType, widgetHandle, 0, 0);
	}
	
	/**
	 * Posts a widget clicked event.
	 * 
	 * @param widgetHandle The widget that was clicked.
	 * @param checked If a checkbox was clicked, this determines if it is checked or not.
	 */
	public void postWidgetClickedEvent(int widgetHandle, boolean checked)
	{
		postWidgetEvent(
			IX_WIDGET.MAW_EVENT_CLICKED, 
			widgetHandle,
			checked ? 1 : 0,
			0);
	}
	
	/**
	 * Posts an event that describes which item in a list that was clicked.
	 * 
	 * @param widgetHandle The list that sends the event.
	 * @param position The position in the list of the view.
	 */
	public void postWidgetItemClickedEvent(int widgetHandle, int position)
	{
		postWidgetEvent(
			IX_WIDGET.MAW_EVENT_ITEM_CLICKED, 
			widgetHandle,
			position,
			0);
	}
	
	/**
	 * Sends a tab changed event from the given tabScreen.
	 * 
	 * @param tabScreen The tab screen that sends the event.
	 * @param newTabIndex The index of the new tab.
	 */
	public void postWidgetTabChangedEvent(int tabScreen, int newTabIndex)
	{
		postWidgetEvent(
			IX_WIDGET.MAW_EVENT_TAB_CHANGED, 
			tabScreen,
			newTabIndex,
			0);
	}
	
	/**
	 * Posts an event from a stack screen has been popped, and a handle to the
	 * screen that was popped.
	 * 
	 * @param stackScreenHandle The stack screen that has been popped.
	 * @param poppedFromScreenHandle The screen that was popped from.
	 * @param poppedToScreenHandle The next screen that will be shown.
	 */
	public void postWidgetStackScreenPoppedEvent(
		int stackScreenHandle, 
		int poppedFromScreenHandle, 
		int poppedToScreenHandle)
	{
		postWidgetEvent(
			IX_WIDGET.MAW_EVENT_STACK_SCREEN_POPPED, 
			stackScreenHandle,
			poppedFromScreenHandle,
			poppedToScreenHandle);
	}
	
	public static EventQueue getDefault()
	{
		return DEFAULT;
	}
}

