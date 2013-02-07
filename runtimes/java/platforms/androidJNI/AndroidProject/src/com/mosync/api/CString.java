package com.mosync.api;

import com.mosync.internal.android.extensions.CStringImpl;


/**
 * A class representing a C string that can be shared
 * between MoSync memory and an Android extension
 * @author Mattias Bybro
 *
 */
public abstract class CString {

	/**
	 * Creates a zero-terminated {@code CString} from a byte array.
	 * @param ptr
	 * @return
	 */
	public static CString fromByteArray(Pointer<Byte> ptr) {
		return new CStringImpl(ptr.getAddress());
	}

	public abstract Pointer<Byte> getPointer();

	public abstract void set(String str, int maxLen);

}
