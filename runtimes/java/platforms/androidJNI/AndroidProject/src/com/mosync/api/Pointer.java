package com.mosync.api;

public interface Pointer<T> {

	T deref();
	T at(int ix);

}
