/* Copyright (C) 2010 MoSync AB

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

#ifndef _REF_COUNTED_H_
#define _REF_COUNTED_H_

class RefCounted {
public:
	// Initializes the reference counter to initialValue (default 1).
	RefCounted(int initialValue=1) : mCount(initialValue) { }
	
	// Increases the reference counter by one.
	void addRef() { 
		mCount++; 
	}

	// Decreases the reference counter by one. If the reference counter equals zero the object is deleted.
	void deleteRef() { 
		mCount--; 
		if(mCount==0) 
			delete this; 
	}
	
	// Equivalent to deleteRef.
	void release() {
		deleteRef();
	}
	
private:
	int mCount;
};

#endif
