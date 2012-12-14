package com.mosync.internal.android.extensions;

import java.nio.ByteBuffer;

import com.mosync.internal.android.MoSyncThread;

public class PointerType extends TypeDescriptor {
	private class PointerImpl implements Pointer {

		private int addr;

		PointerImpl(int addr) {
			this.addr = addr;
		}

		@Override
		public Object deref() {
			return at(0);
		}

		@Override
		public Object at(int ix) {
			int size = delegate.size();
			ByteBuffer sliceBuf = MoSyncThread.getInstance().getMemorySlice(addr + 4 * ix * size, 4 * size);
			int[] slice = new int[size];
			sliceBuf.order(null).asIntBuffer().get(slice);
			return delegate.convert(slice, 0);
		}

	}

	private TypeDescriptor delegate;

	public PointerType(TypeDescriptor delegate) {
		this.delegate = delegate;
	}

	@Override
	public Class getNativeClass() {
		return Pointer.class;
	}

	@Override
	public Object convert(int[] args, int offset) {
		int addr = args[offset];
		return new PointerImpl(addr);
	}
}