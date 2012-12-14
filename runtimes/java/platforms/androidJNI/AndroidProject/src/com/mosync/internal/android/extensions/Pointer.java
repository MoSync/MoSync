package com.mosync.internal.android.extensions;

public interface Pointer<T> {

	T deref();
	T at(int ix);

}
