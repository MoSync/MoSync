package com.mosync.api;

import com.mosync.internal.android.extensions.CStringImpl;
import com.mosync.internal.android.extensions.PointerImpl;
import com.mosync.internal.android.extensions.StringType;
import com.mosync.internal.android.extensions.VoidType;
import com.mosync.internal.android.extensions.WStringImpl;


/**
 * A class representing a C string that can be shared
 * between MoSync memory and an Android extension
 * @author Mattias Bybro
 *
 */
public abstract class CString {

	public static enum Storage {
		CHAR, WCHAR
	}


	protected int addr;
	protected int maxLen;
	protected boolean resolved = false;
	protected String str;
	private Pointer<Void> endPtr;

	/**
	 * Creates a zero-terminated {@code CString} from a pointer.
	 * @param ptr The pointer being used
	 * @param storage the type of storage used
	 * @param maxLen The maximum length of the string, or {@code -1} if undefined
	 * @return
	 */
	public static CString fromByteArray(Pointer<Void> ptr, Storage storage, int maxLen) {
		return storage == Storage.CHAR ?
			new CStringImpl(ptr.getAddress(), maxLen) :
			new WStringImpl(ptr.getAddress(), maxLen);
	}

	public static int getCharSize(Storage storage) {
		return StringType.getCharSize(storage);
	}

	protected CString(int addr, int maxLen) {
		this.addr = addr;
		this.maxLen = maxLen;
	}

	public Pointer<Void> getPointer() {
		return new PointerImpl(VoidType.getInstance(), addr);
	}

	public Pointer<Void> getEnd() {
		internalResolve();
		return endPtr;
	}

	public String toString() {
		internalResolve();
		return str;
	}

	private void internalResolve() {
		if (!resolved) {
			str = resolve();
			int lenInBytes = str == null ? 0 : (str.length() + 1) * getCharSize(getStorage());
			endPtr = new PointerImpl(VoidType.getInstance(), addr + lenInBytes);
			resolved = true;
		}
	}

	public abstract void set(String str, int maxLen);

	public abstract Storage getStorage();

	protected abstract String resolve();

}
