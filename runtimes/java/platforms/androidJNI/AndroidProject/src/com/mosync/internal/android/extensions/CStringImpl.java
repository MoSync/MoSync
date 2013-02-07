package com.mosync.internal.android.extensions;

import com.mosync.api.CString;
import com.mosync.api.Pointer;

public class CStringImpl extends CString {

	private int addr;
	private boolean resolved = false;
	private String str;

	public CStringImpl(int addr) {
		this.addr = addr;
	}

	public String toString() {
		if (!resolved) {
			resolved = true;
			str = StringType.unmarshalString(addr, -1);
		}
		return str;
	}

	public void set(String str, int maxLen) {
		StringType.marshalString(addr, str, maxLen);
	}

	public Pointer<Byte> getPointer() {
		return new PointerImpl(CharType.getInstance(), addr);
	}

	int getAddress() {
		return addr;
	}
}
