/* Copyright (C) 2010 Mobile Sorcery AB

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

/** 
* \file MapSourceMgr.h
* \brief Switch between map sources
* \author Lars-Åke Vinberg
*/

#ifndef MAPSOURCEMGR_H_
#define MAPSOURCEMGR_H_

#include <MAUtil/Map.h>

#include "MapSource.h"

namespace MAP
{
	//=========================================================================
	/**
	 * Manages map sources for clients to access.
	 * Implemented as singleton.
	 */
	class MapSourceMgr
	//=========================================================================
	{
	private:
		MapSourceMgr( );

	public:
		virtual ~MapSourceMgr( );
		/**
		 * Client accesses singleton through MapSourceMgr::get( )
		 */
		static MapSourceMgr* get( );
		static void shutdown( );
		/*
		 * Returns map source
		 */
		MapSource* getMapSource( MapSourceKind kind );

	private:
		static MapSourceMgr* sSingleton;
		Map<MapSourceKind, MapSource*> mMap;
	};
}

#endif // MAPSOURCEMGR_H_

