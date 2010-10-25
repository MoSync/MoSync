/*
 * netbase.h
 *
 *  Created on: 25 okt 2010
 *      Author: Ali Mosavian
 */

#ifndef __NETBASE_H__
#define __NETBASE_H__

class NetBase
{
protected:
	static const int MAX_DELAY;

	/**
 	 * Waits for a network event.
	 *
	 * @param ms 		Maximum number of milliseconds to wait
	 * @param opType 	Connection even type
	 *
	 * @return True if event was caught with the given time limit.
	 */
	bool waitForEvent ( int ms, int opType );

	/**
	 * Waits for a network event and returns the event
	 * return code
	 * @param ms 		Maximum number of milliseconds to wait.
	 * @param opType 	Connection even type.
	 * @param result    Event result code [out].
	 *
	 * @return True if event was caught with the given time limit.
	 */
	bool waitForEvent ( int ms, int opType, int &result );
};

#endif /* __NETBASE_H__ */
