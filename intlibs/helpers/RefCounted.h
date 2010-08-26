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
