/* Copyright (C) 2012 MoSync AB

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

#include "EventQueue.h"

#define MIKILOG(a) __android_log_write(ANDROID_LOG_INFO, "@@@ MoSync", a);

static int GetSafePointerId(int pointerId)
{
	if (pointerId >= 0 &&
		pointerId < ANDROID_EVENTQUEUE_POINTERARRAYSIZE)
	{
		return pointerId;
	}
	else
	{
		return 0;
	}
}

static int GetSafeSensorId(int sensorId)
{
	if (sensorId >= 0 &&
		sensorId < ANDROID_EVENTQUEUE_SENSORARRAYSIZE)
	{
		return sensorId;
	}
	else
	{
		return 0;
	}
}

static void PrintPointerEvent(const MAEvent& event)
{
	char buf[512];
	sprintf(
		buf,
		"  PointerDraggedEvent: %d %d %d",
		event.point.x,
		event.point.y,
		event.touchId);
	MIKILOG(buf);
}

EventQueue::EventQueue()
{
	InitializeCriticalSection(&mCriticalSection);

	for (int i = 0; i < ANDROID_EVENTQUEUE_POINTERARRAYSIZE; ++i)
	{
		mLastPointerDraggedEvents[i] = NULL;
	}

	for (int i = 0; i < ANDROID_EVENTQUEUE_SENSORARRAYSIZE; ++i)
	{
		mLastSensorEvents[i] = NULL;
	}
}

void EventQueue::put(MAEvent& event)
{
	CriticalSectionHandler section(&mCriticalSection);

	if (EVENT_TYPE_POINTER_DRAGGED == event.type)
	{
		// We got a pointer dragged event.

		int pointerId = GetSafePointerId(event.touchId);

		if (NULL != mLastPointerDraggedEvents[pointerId])
		{
			MIKILOG("OLD Pointer Event");
			PrintPointerEvent(event);

			// If there is already an event of this type in the queue,
			// we update it by copying the new data to it.
			memcpy(
				mLastPointerDraggedEvents[pointerId],
				&event,
				sizeof(MAEvent));

			PrintPointerEvent(*(mLastPointerDraggedEvents[pointerId]));
		}
		else
		{
			MIKILOG("NEW Pointer Event");
			PrintPointerEvent(event);

			// If there is no event of this type in the queue,
			// we save a pointer to it and insert the event.
			mLastPointerDraggedEvents[pointerId] = &event;
			mFifo.put(event);
		}
	}
	else if (EVENT_TYPE_SENSOR == event.type)
	{
		// We got a sensor event.

		int sensorId = GetSafeSensorId(event.sensor.type);

		if (NULL != mLastSensorEvents[sensorId])
		{
			MIKILOG("OLD Sensor Event ******");

			// If there is already an event of this type in the queue,
			// we update it by copying the new data to it.
			// Since there are many types of sensor events, we save
			// a pointer to each subtype.
			memcpy(
				mLastSensorEvents[sensorId],
				&event,
				sizeof(MAEvent));
		}
		else
		{
			MIKILOG("NEW Sensor Event");

			// If there is no event of this type in the queue,
			// we save a pointer to it and insert the event.
			mLastSensorEvents[sensorId] = &event;
			mFifo.put(event);
		}
	}
	else
	{
		// For all other events, we just add them to the queue.
		mFifo.put(event);
	}
}

const MAEvent& EventQueue::get()
{
	CriticalSectionHandler section(&mCriticalSection);

	// Get the next event in the queue.
	const MAEvent& event = mFifo.get();

	// If the event is one of the types that we are handling,
	// we reset the pointer to the last event to NULL when they
	// are retrieved.
	if (EVENT_TYPE_POINTER_DRAGGED == event.type)
	{
		MIKILOG("REMOVE Pointer Event");
		PrintPointerEvent(event);

		int pointerId = GetSafePointerId(event.touchId);
		mLastPointerDraggedEvents[pointerId] = NULL;
	}
	else if (EVENT_TYPE_SENSOR == event.type)
	{
		int sensorId = GetSafePointerId(event.sensor.type);
		mLastSensorEvents[sensorId] = NULL;
	}

	// We always return the event, regardless of the type.
	return event;
}

size_t EventQueue::count()
{
	CriticalSectionHandler section(&mCriticalSection);
	return mFifo.count();
}

void EventQueue::clear()
{
	CriticalSectionHandler section(&mCriticalSection);
	return mFifo.clear();
}
