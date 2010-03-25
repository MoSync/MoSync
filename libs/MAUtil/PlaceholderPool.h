/* Copyright (C) 2009 Mobile Sorcery AB

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

/** \file PlaceholderPool.h
* \brief A dynamic pool of MoSync placeholder resource handles.
*/

#ifndef _SE_MSAB_MAUTIL_PLACEHOLDERPOOL_H_
#define _SE_MSAB_MAUTIL_PLACEHOLDERPOOL_H_

#include <ma.h>

namespace MAUtil {

/**
* A dynamic pool of MoSync placeholder resource handles.
*
* Such handles are used to construct new MoSync resources,
* such as images, sound clips or data objects.
*/
namespace PlaceholderPool {
	/**
	* Retrieves and removes a MAHandle from the pool, or,
	* if the pool is empty, allocates a new MAHandle.
	*/
	MAHandle alloc();

	/**
	* Adds a MAHandle to the pool. If the MAHandle refers to a valid MoSync Object,
	* that object is destroyed.
	*/
	void put(MAHandle h);
}

}

#endif	//_SE_MSAB_MAUTIL_PLACEHOLDERPOOL_H_
