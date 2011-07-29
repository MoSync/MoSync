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

#ifndef HASH_MAP_H
#define HASH_MAP_H

#ifdef __GNUC__
#ifdef HAVE_TR1
#include <unordered_map>
#define hash_map std::unordered_map
#else
#include <ext/hash_map>
using namespace __gnu_cxx;
#include "hash_compare.h"
#endif	//HAVE_TR1
#elif defined(_MSC_VER)
#include <hash_map>
using namespace stdext;
#else
#error Unsupported platform!
#endif

#endif	//HASH_MAP_H
