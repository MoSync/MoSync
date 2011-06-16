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
