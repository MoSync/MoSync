package com.mosync.internal.android.extensions;

import com.mosync.api.Pointer;

public class PointerImpl extends Pointer {

	private int addr;
	private TypeDescriptor delegate;

	PointerImpl(TypeDescriptor delegate, int addr) {
		this.delegate = delegate;
		this.addr = addr;
	}

	@Override
	public Object deref() {
		if (addr == 0) {
			return null;
		}
		return at(0);
	}

	@Override
	public Object at(int ix) {
		int size = delegate.size();
		return delegate.readFromMemory(addr + ix * size);
	}

	protected int getAddress() {
		return addr;
	}

	@Override
	public void set(Object o) {
		set(o, 0);
	}

	@Override
	public void set(Object o, int ix) {
		int size = delegate.size();
		delegate.writeToMemory(addr + ix * size, o);
	}

}