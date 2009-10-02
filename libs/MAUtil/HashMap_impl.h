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

#ifndef _SE_MSAB_MAUTIL_HASHMAP_H_
#error Don't include this file directly.
#endif

//******************************************************************************
// THashFunction
//******************************************************************************

template<> hash_val_t THashFunction(const String& s) {
	return hash_fun_default(s.c_str());
}

template<> hash_val_t THashFunction(const int&) {
	int key = *(int*)data;
	key = key + ~(key << 15); // key = key - (key << 15) - 1;
	key = key ^ (key >> 10);
	key = key + (key << 3);
	key = key ^ (key >> 6);
	key = key * 16389; // key = (key + (key << 2)) + (key << 14);
	key = key ^ (key >> 16);
	return key;
}

//******************************************************************************
// HashMap
//******************************************************************************


//******************************************************************************
// Iterator
//******************************************************************************

//******************************************************************************
// ConstIterator
//******************************************************************************
