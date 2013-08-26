/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
