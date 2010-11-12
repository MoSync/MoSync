/*
 * EventBase.cpp
 *
 * Created on: November 11, 2010
 * Author: Mikael Kindborg
 */
#include <maapi.h>
#include "eventbase.h"

// Maximum delay for an event in milliseconds
const int EventBase::MAX_DELAY = 30*1000;

/**
 * Waits for an event.
 *
 * @param millisecondTimeout Maximum number of milliseconds to wait.
 * @param eventType Event type to wait for.
 *
 * @return True if event was caught with the given time limit.
 */
bool EventBase::waitForEvent(int millisecondTimeout, int eventType)
{
	MAEvent returnedEvent;
	return waitForEvent (millisecondTimeout, eventType, returnedEvent);
}

/**
 * Waits for an event and returns the event object.
 *
 * @param millisecondTimeout Maximum number of milliseconds to wait.
 * @param eventType Event type to wait for.
 * @param returnedEvent Event object [out].
 *
 * @return True if event was caught with the given time limit.
 */
bool EventBase::waitForEvent(int millisecondTimeout, int eventType, MAEvent &returnedEvent)
{
	MAEvent event;
	long startTime = maGetMilliSecondCount();

	while (maGetMilliSecondCount() - startTime < millisecondTimeout)
	{
		maWait(100);
		while(maGetEvent(&event) != 0)
		{
			if (event.type == eventType)
			{
				returnedEvent = event;
				return true;
			}
			else if (event.type == EVENT_TYPE_CLOSE)
			{
				maExit(0);
			}
		}
	}

	return false;
}


