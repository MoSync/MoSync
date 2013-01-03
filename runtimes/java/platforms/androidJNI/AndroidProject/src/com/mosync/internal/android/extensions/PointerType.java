package com.mosync.internal.android.extensions;

import java.nio.ByteBuffer;

import com.mosync.api.Pointer;
import com.mosync.internal.android.MoSyncThread;

public class PointerType extends TypeDescriptor {
	TypeDescriptor delegate;

	public PointerType(TypeDescriptor delegate) {
		this.delegate = delegate;
	}

	@Override
	public Class getNativeClass() {
		return Pointer.class;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		return new PointerImpl(delegate, IntType.unmarshalInt(data, offset));
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		PointerImpl ptr = (PointerImpl) o;
		int addr = ptr.getAddress();
		IntType.marshalInt(addr, data, offset);
	}

	@Override
	public int size() {
		return 4;
	}

}