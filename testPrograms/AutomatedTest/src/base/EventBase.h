/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
