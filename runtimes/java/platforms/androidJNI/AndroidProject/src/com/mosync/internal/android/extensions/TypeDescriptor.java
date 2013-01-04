package com.mosync.internal.android.extensions;

import java.nio.ByteBuffer;
import java.util.HashSet;

import com.mosync.api.Marshaller;
import com.mosync.internal.android.MoSyncThread;

public abstract class TypeDescriptor implements Marshaller {

	private boolean out;

	public abstract Class getNativeClass();

	public abstract int size();

	public Object readFromMemory(int addr) {
		byte[] buf = new byte[size()];
		ByteBuffer mem = getMemorySlice(addr, size());
		mem.get(buf);
		return unmarshal(buf, 0);
	}

	public void writeToMemory(int addr, Object o) {
		byte[] buf = new byte[size()];
		ByteBuffer mem = getMemorySlice(addr, size());
		marshal(o, buf, 0);
		mem.put(buf);
	}

	/**
	 * Returns a memory slice with the correct endianness
	 * @param addr
	 * @param len
	 * @return
	 */
	protected static ByteBuffer getMemorySlice(int addr, int len) {
		return MoSyncThread.getInstance().getMemorySlice(addr, len).order(null);
	}


	public String toString() {
		return getNativeClass().getSimpleName();
	}

	public void setOut(boolean out) {
		this.out = out;
	}

	public boolean isOut() {
		return out;
	}

	public boolean isPrimitive() {
		return getNativeClass().isPrimitive();
	}

}