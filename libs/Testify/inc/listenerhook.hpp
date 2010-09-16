/* Copyright (C) 2009 Ali Mosavian

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
 * listenerhook.hpp
 *
 *  Created on: 31 mar 2010
 *      Author: Ali Mosavian
 */

#ifndef __LISTENERHOOK_HPP__
#define __LISTENERHOOK_HPP__

#include "common.h"
#include "testlistener.hpp"
#include "testmanager.hpp"


NAMESPACE_BEGIN( Testify )

/**
 * @brief A hook for adding a listener to test run.
 * It uses global constructors to add the listener to the
 * test manager.
 *
 */
class ListenerHook
{
public:
	/**
	 * Adds a listener to the test manager
	 * Note: The listener will be deleted by the test manager.
	 *
	 * @param l Pointer to the listener that is to be used.
	 */
	ListenerHook ( TestListener *l )
	{
		TestManager::getInstance( )->addTestListener( l );
	}
};

NAMESPACE_END


#endif /* __LISTENERHOOK_HPP__ */
