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

#ifndef HASH_SET_H
#define HASH_SET_H

#ifdef __GNUC__
#ifdef HAVE_TR1
#include <unordered_set>
#define hash_set std::unordered_set
#define hash_compare std::hash
#else
#include <ext/hash_set>
using namespace __gnu_cxx;
#if 1
namespace __gnu_cxx {
	template<> struct hash<std::string>
	{
		size_t operator()(const std::string& __s) const
		{ return __stl_hash_string(__s.c_str()); }
	};
}
#endif	//1
template<class T> class hash_compare : public hash<T> {};
#endif	//HAVE_TR1
#elif defined(_MSC_VER)
#include <hash_set>
using namespace stdext;
#else
#error Unsupported platform!
#endif

#endif	//HASH_SET_H
