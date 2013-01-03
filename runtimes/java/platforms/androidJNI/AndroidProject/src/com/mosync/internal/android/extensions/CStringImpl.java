package com.mosync.internal.android.extensions;

import com.mosync.api.CString;

public class CStringImpl implements CString {

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

	int getAddress() {
		return addr;
	}
}
