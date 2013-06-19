package com.mosync.internal.android.extensions;

import com.mosync.api.CString;
import com.mosync.api.Pointer;

public class CStringImpl extends CString {

	public CStringImpl(int addr, int maxLen) {
		super(addr, maxLen);
	}

	protected String resolve() {
		return StringType.unmarshalCString(addr, maxLen);
	}

	public void set(String str, int maxLen) {
		StringType.marshalCString(addr, str, maxLen);
	}

	@Override
	public Storage getStorage() {
		return Storage.CHAR;
	}
}
