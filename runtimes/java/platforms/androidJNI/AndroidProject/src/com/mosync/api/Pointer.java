package com.mosync.api;

public abstract class Pointer<T> {

	public abstract T deref();
	public abstract T at(int ix);

	public abstract void set(T o);
	public abstract void at(T o, int ix);

	protected abstract int getAddress();
}
