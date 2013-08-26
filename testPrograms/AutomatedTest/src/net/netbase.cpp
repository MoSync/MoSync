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



