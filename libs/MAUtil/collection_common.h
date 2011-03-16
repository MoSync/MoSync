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

/** \file collection_common.h
 * \brief Compare() and Pair for MAUtil.
 */

#ifndef _SE_MSAB_MAUTIL_COLLECTION_COMMON_H_
#define _SE_MSAB_MAUTIL_COLLECTION_COMMON_H_

#ifndef NULL
#define NULL 0
#endif

#ifndef OFFSETOF
#define OFFSETOF(struct, member) ((int)(((char*)&(((struct*)1)->member)) - 1))
#endif

/**
* \brief MoSync utility libraries
*/
namespace MAUtil {

//******************************************************************************
// Comparator
//******************************************************************************

/** \brief Template comparator function.
* Returns -1 if \a a \< \a b,
* 0 if \a a == \a b
* and 1 if \a a \> \a b.
*/
template<class T> int Compare(const T& a, const T& b) {
	if(a < b)
		return -1;
	else if(a == b)
		return 0;
	else
		return 1;
}

//******************************************************************************
// Pair
//******************************************************************************

/// Template storage for a pair of objects.
template<class F, class S> struct Pair {
	F first;
	S second;

	Pair() {}
	Pair(const F& f, const S& s) : first(f), second(s) {}

	template<class OF, class OS>
	Pair(const Pair<OF, OS>& o) : first(o.first), second(o.second) {}
};

}	//namespace MAUtil

#endif	//_SE_MSAB_MAUTIL_COLLECTION_COMMON_H_
