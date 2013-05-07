package com.mosync.api;

import com.mosync.internal.android.extensions.PointerImpl;
import com.mosync.internal.android.extensions.VoidType;

public abstract class Pointer<T> {

	public abstract T deref();
	public abstract T at(int ix);

	public abstract void set(T o);
	public abstract void set(T o, int ix);

	public abstract int getAddress();

	public abstract boolean isNull();

	/**
	 * Creates a {@code Pointer} with the {@code Void}
	 * based on this {@code Pointer}'s address.
	 * @param offset The offset in bytes
	 * @return A new {@code Pointer}
	 * @throws @{@link NullPointerException} if this is a {@code NULL} pointer
	 */
	public abstract Pointer<Void> offset(int offset);

	/**
	 * Creates a {@code Pointer} with the {@code Void}.
	 * @param buffPointer
	 * @return
	 */
	public static Pointer<Void> createRawPointer(int addr) {
		return new PointerImpl(VoidType.getInstance(), addr);
	}
}
