/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
