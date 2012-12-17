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
		return new PointerImpl(this, IntType.unmarshalInt(data, offset));
	}

	@Override
	public int size() {
		return 4;
	}
}