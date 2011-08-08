/*
Copyright (C) 2011 MoSync AB

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
/*
 * netbase.cpp
 *
 *  Created on: 25 okt 2010
 *      Author: Ali Mosavian
 */
#include <maapi.h>
#include "netbase.h"


// Maximum delay for an event in ms
const int NetBase::MAX_DELAY = 30*1000;


/**
 * Waits for a network event.
 *
 * @param ms 		Maximum number of milliseconds to wait
 * @param opType 	Connection even type
 *
 * @return True if event was caught with the given time limit.
 */
bool NetBase::waitForEvent ( int ms, int opType )
{
	int result;
	return waitForEvent ( ms, opType, result );
}


/**
 * Waits for a network event and returns the event
 * return code.
 *
 * @param ms 		Maximum number of milliseconds to wait.
 * @param opType 	Connection event type.
 * @param result    Event result code [out].
 *
 * @return True if event was caught with the given time limit.
 */
bool NetBase::waitForEvent ( int ms, int opType, int &result )
{
	MAEvent event;
	long startTime = maGetMilliSecondCount( );

	while ( maGetMilliSecondCount( )-startTime < ms )
	{
		maWait( 100 );
		while( maGetEvent( &event ) != 0 )
		{
			if ( event.type == EVENT_TYPE_CONN &&
				 event.conn.opType == opType )
			{
				result = event.conn.result;
				return true;
			}

			if ( event.type == EVENT_TYPE_CLOSE )
			{
				maExit(0);
			}
		}
	}

	return false;
}



