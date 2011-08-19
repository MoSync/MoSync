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

	/**
	 * Post a widget event.
	 * 
	 * @param widgetEventType The type of the widget event.
	 * @param widgetHandle The handle of the widget.
	 * @param auxParam1 Parameter used by some (used by some
	 * messages for parameters like messageDataHandle, listItemIndex,
	 * searchBarButton, etc. See struct MAWidgetEventData for
	 * documentation of these fields.
	 * @param auxParam2 Parameter used by some messages.
	 * Set to zero if not used.
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
	 * @param checked If a checkbox or toggle button was clicked, this determines if it is checked or not.
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

