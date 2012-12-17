package com.mosync.internal.android.extensions;

import java.nio.ByteBuffer;
import java.util.HashSet;

import com.mosync.api.Marshaller;
import com.mosync.internal.android.MoSyncThread;

public abstract class TypeDescriptor implements Marshaller {
	public abstract Class getNativeClass();

	public abstract int size();

	public boolean isArray() {
		return false;
	}

	public final TypeDescriptor resolve() {
		return resolve(new HashSet<String>());
	}

	public TypeDescriptor resolve(HashSet<String> circularCheck) {
		return this;
	}

	public Object readFromMemory(int addr) {
		byte[] buf = new byte[size()];
		ByteBuffer mem = getMemorySlice(addr, size());
		mem.get(buf);
		return unmarshal(buf, 0);
	}

	/**
	 * Returns a memory slice with the correct endianness
	 * @param addr
	 * @param len
	 * @return
	 */
	protected ByteBuffer getMemorySlice(int addr, int len) {
		return MoSyncThread.getInstance().getMemorySlice(addr, len).order(null);
	}


	public String toString() {
		return getNativeClass().getSimpleName();
	}

}