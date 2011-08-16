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
	
	/**
	 * Posts a widget clicked event.
	 * 
	 * @param widgetHandle The widget that was clicked.
	 * @param checked If a checkbox or toggle button was clicked, this determines if it is checked or not.
	 */
	public void postWidgetClickedEvent(int widgetHandle, boolean checked)
	{
		int event[] = new int[ 5 ];
		
		event[0] = EVENT_TYPE_WIDGET;
		event[1] = IX_WIDGET.MAW_EVENT_CLICKED;
		event[2] = widgetHandle;
		event[3] = checked ? 1 : 0;
		
		sMoSyncThread.postEvent( event );
	}
	
	/**
	 * Posts an event that describes which item in a list that was clicked.
	 * 
	 * @param widgetHandle The list that sends the event.
	 * @param position The position in the list of the view.
	 */
	public void postWidgetItemClickedEvent(int widgetHandle, int position)
	{
		int event[] = new int[ 5 ];
		
		event[0] = EVENT_TYPE_WIDGET;
		event[1] = IX_WIDGET.MAW_EVENT_ITEM_CLICKED;
		event[2] = widgetHandle;
		event[3] = position;
		
		sMoSyncThread.postEvent( event );
	}
	
	/**
	 * Sends a tab changed event from the given tabScreen.
	 * 
	 * @param tabScreen The tab screen that sends the event.
	 * @param newTabIndex The index of the new tab.
	 */
	public void postWidgetTabChangedEvent(int tabScreen, int newTabIndex)
	{
		int event[] = new int[ 5 ];
		
		event[0] = EVENT_TYPE_WIDGET;
		event[1] = IX_WIDGET.MAW_EVENT_TAB_CHANGED;
		event[2] = tabScreen;
		event[3] = newTabIndex;
		
		sMoSyncThread.postEvent( event );
	}
	
	/**
	 * Posts an event from a stack screen has been popped, and a handle to the
	 * screen that was popped.
	 * 
	 * @param stackScreenHandle The stack screen that has been popped.
	 * @param poppedFromScreenHandle The screen that was popped from.
	 * @param poppedToScreenHandle The next screen that will be shown.
	 */
	public void postWidgetStackScreenPoppedEvent(int stackScreenHandle, 
			int poppedFromScreenHandle, int poppedToScreenHandle)
	{
		int event[] = new int[ 5 ];
		
		event[0] = EVENT_TYPE_WIDGET;
		event[1] = IX_WIDGET.MAW_EVENT_STACK_SCREEN_POPPED;
		event[2] = stackScreenHandle;
		event[3] = poppedFromScreenHandle;
		event[4] = poppedToScreenHandle;
		
		sMoSyncThread.postEvent( event );
	}

	/**
	 * Sends a value changed event from the given slider.
	 *
	 * @param widgetHandle The slider that sends the event.
	 * @param value The value of the slider.
	 */
	public void postSeekBarValueChangedEvent(int widgetHandle, int value)
	{
		int event[] = new int[ 5 ];

		event[0] = EVENT_TYPE_WIDGET;
		event[1] = IX_WIDGET.MAW_EVENT_SLIDER_VALUE_CHANGED;
		event[2] = widgetHandle;
		event[3] = value;

		sMoSyncThread.postEvent( event );
	}

	/**
	 * Sends a value changed event from the given date picker.
    *
	 * @param widgetHandle The date picker that sends the event.
	 * @param dayOfMonthValue The value selected for the day.
	 * @param monthValue The value selected for the month.
	 * @param yearValue The value selected for the year.
	 */
	public void postDatePickerValueChangedEvent(int widgetHandle, int dayOfMonthValue, int monthValue, int yearValue)
	{
		int event[] = new int[ 6 ];

		event[0] = EVENT_TYPE_WIDGET;
		event[1] = IX_WIDGET.MAW_EVENT_DATE_PICKER_VALUE_CHANGED;
		event[2] = widgetHandle;
		event[3] = dayOfMonthValue;
		event[4] = monthValue;
		event[5] = yearValue;

		sMoSyncThread.postEvent( event );
	}

	/**
	 * Sends a value changed event from the given time picker.
     *
	 * @param widgetHandle The time picker that sends the event.
	 * @param currentHour The value selected for the hour.
	 * @param currentMinute The value selected for the minute.
	 */
	public void postTimePickerValueChangedEvent(int widgetHandle, int currentHour, int currentMinute)
	{
		int event[] = new int[ 5 ];

		event[0] = EVENT_TYPE_WIDGET;
		event[1] = IX_WIDGET.MAW_EVENT_TIME_PICKER_VALUE_CHANGED;
		event[2] = widgetHandle;
		event[3] = currentHour;
		event[4] = currentMinute;

		sMoSyncThread.postEvent( event );
	}

	/**
	 * Sends a value changed event from the given number picker.
	 *
	 * @param widgetHandle The number picker that sends the event.
	 * @param newValue The selected value.
	 */
	public void postNumberPickerValueChangedEvent(int widgetHandle, int newValue)
	{
		int event[] = new int[ 4 ];

		event[0] = EVENT_TYPE_WIDGET;
		event[1] = IX_WIDGET.MAW_EVENT_NUMBER_PICKER_VALUE_CHANGED;
		event[2] = widgetHandle;
		event[3] = newValue;

		sMoSyncThread.postEvent( event );
	}

	/**
	 * Sends the new state of the video view.
	 * @param widgetHandle The video view that sends the event.
	 * @param state One of the MAW_VIDEO_WIDGET_STATE constants.
	 */
	public void postVideoStateChanged(int widgetHandle, int state)
	{
		int event[] = new int[4];

		event[0] = EVENT_TYPE_WIDGET;
		event[1] = IX_WIDGET.MAW_EVENT_VIDEO_STATE_CHANGED;
		event[2] = widgetHandle;
		event[3] = state;

		sMoSyncThread.postEvent(event);
	}

	public static EventQueue getDefault()
	{
		return DEFAULT;
	}
}

