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

#ifndef _SE_MSAB_MAUTIL_COLLECTION_COMMON_H_
#define _SE_MSAB_MAUTIL_COLLECTION_COMMON_H_

#ifndef NULL
#define NULL 0
#endif

namespace MAUtil {

//******************************************************************************
// Comparator
//******************************************************************************

/**
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

template<class F, class S> struct Pair {
	F first;
	S second;
};

}	//namespace MAUtil

#endif	//_SE_MSAB_MAUTIL_COLLECTION_COMMON_H_
