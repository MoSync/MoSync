/*
 * EventBase.cpp
 *
 * Created on: November 11, 2010
 * Author: Mikael Kindborg
 */

#ifndef __EVENTBASE_H__
#define __EVENTBASE_H__

class EventBase
{
public:
	static const int MAX_DELAY;

	/**
	 * Waits for an event.
	 *
	 * @param millisecondTimeout Maximum number of milliseconds to wait.
	 * @param eventType Event type to wait for.
	 *
	 * @return True if event was caught with the given time limit.
	 */
	bool waitForEvent(int millisecondTimeout, int eventType);

	/**
	 * Waits for an event and returns the event object.
	 *
	 * @param millisecondTimeout Maximum number of milliseconds to wait.
	 * @param eventType Event type to wait for.
	 * @param returnedEvent Event object [out].
	 *
	 * @return True if event was caught with the given time limit.
	 */
	bool waitForEvent(int millisecondTimeout, int eventType, MAEvent &returnedEvent);
};

#endif
