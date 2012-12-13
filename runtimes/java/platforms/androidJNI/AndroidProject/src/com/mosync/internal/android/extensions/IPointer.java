package com.mosync.internal.android.extensions;

public interface IPointer<T> {

	T deref();
	T at(int ix);

}
