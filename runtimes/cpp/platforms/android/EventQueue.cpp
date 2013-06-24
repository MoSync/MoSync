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

static int GetSafePointerId(int pointerId)
{
	if (pointerId >= 0 &&
		pointerId < EVENTQUEUE_POINTERARRAYSIZE)
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
		sensorId < EVENTQUEUE_SENSORARRAYSIZE)
	{
		return sensorId;
	}
	else
	{
		return 0;
	}
}

EventQueue::EventQueue()
{
	InitializeCriticalSection(&mCriticalSection);

	for (int i = 0; i < EVENTQUEUE_POINTERARRAYSIZE; ++i)
	{
		mPointerDraggedEvents[i] = EVENTQUEUE_NULL;
	}

	for (int i = 0; i < EVENTQUEUE_SENSORARRAYSIZE; ++i)
	{
		mSensorEvents[i] = EVENTQUEUE_NULL;
	}
}

void EventQueue::put(MAEvent& event)
{
	CriticalSectionHandler section(&mCriticalSection);

	if (EVENT_TYPE_POINTER_DRAGGED == event.type)
	{
		// We got a pointer dragged event.

		int pointerId = GetSafePointerId(event.touchId);

		if (EVENTQUEUE_NULL != mPointerDraggedEvents[pointerId])
		{
			// If there is already an event of this type in the queue,
			// we update it by copying the new data to it.
			mFifo.putAtIndex(
				mPointerDraggedEvents[pointerId],
				event);
		}
		else
		{
			// If there is no event of this type in the queue,
			// we insert the element and save a pointer to it.
			mFifo.put(event);
			mPointerDraggedEvents[pointerId] = mFifo.getLastIndex();
		}
	}
	else if (EVENT_TYPE_SENSOR == event.type)
	{
		// We got a sensor event.

		int sensorId = GetSafeSensorId(event.sensor.type);

		if (EVENTQUEUE_NULL != mSensorEvents[sensorId])
		{
			// If there is already an event of this type in the queue,
			// we update it by copying the new data to it.
			mFifo.putAtIndex(
				mSensorEvents[sensorId],
				event);
		}
		else
		{
			// If there is no event of this type in the queue,
			// we insert the element and save a pointer to it.
			mFifo.put(event);
			mSensorEvents[sensorId] = mFifo.getLastIndex();
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
	// we reset the pointer to the last event to EVENTQUEUE_NULL
	// when they are retrieved.
	if (EVENT_TYPE_POINTER_DRAGGED == event.type)
	{
		int pointerId = GetSafePointerId(event.touchId);
		mPointerDraggedEvents[pointerId] = EVENTQUEUE_NULL;
	}
	else if (EVENT_TYPE_SENSOR == event.type)
	{
		int sensorId = GetSafePointerId(event.sensor.type);
		mSensorEvents[sensorId] = EVENTQUEUE_NULL;
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
