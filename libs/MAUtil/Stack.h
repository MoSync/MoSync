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

/** \file Stack.h
 *
 * \brief A generic, dynamic stack container. A restricted subset of vector.
 *
 * \author Niklas Nummelin
 *
 */
 
#ifndef _SE_MSAB_MAUTIL_STACK_H_
#define _SE_MSAB_MAUTIL_STACK_H_

#include "Vector.h"

namespace MAUtil {

template <typename T>
class Stack {
	public:
		void clear() {
			mData.clear();
		}
		
		const T& peek() const {
			return mData[mData.size()-1];
		}
		
		T& peek() {
			return mData[mData.size()-1];
		}	

		void pop() {
			mData.resize(mData.size()-1);
		}
		
		void push(const T& d) {
			mData.add(d);
		}
		
		int size() const { 
			return mData.size();
		}
		
		bool empty() const {
			return mData.size()==0;
		}
		
	private:
		Vector<T> mData;
};

} // MAUtil

#endif
