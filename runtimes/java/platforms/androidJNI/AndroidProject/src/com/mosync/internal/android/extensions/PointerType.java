package com.mosync.internal.android.extensions;

import java.nio.ByteBuffer;

import com.mosync.internal.android.MoSyncThread;

public class PointerType extends TypeDescriptor {
	private TypeDescriptor delegate;

	public PointerType(TypeDescriptor delegate) {
		this.delegate = delegate;
	}

	@Override
	public Class getNativeClass() {
		return IPointer.class;
	}

	@Override
	public Object convert(int[] args, int offset) {
		int addr = args[offset];
		int size = delegate.size();
		ByteBuffer sliceBuf = MoSyncThread.getInstance().getMemorySlice(addr, 4 * size);
		int[] slice = new int[size];
		sliceBuf.asIntBuffer().get(slice);
		return delegate.convert(args, 0);
	}
}