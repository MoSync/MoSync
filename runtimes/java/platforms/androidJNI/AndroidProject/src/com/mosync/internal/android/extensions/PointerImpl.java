package com.mosync.internal.android.extensions;

import com.mosync.api.Pointer;

public class PointerImpl implements Pointer {

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

}