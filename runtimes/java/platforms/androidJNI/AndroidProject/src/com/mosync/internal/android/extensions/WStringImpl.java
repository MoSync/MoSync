package com.mosync.internal.android.extensions;

import com.mosync.api.CString;

public class WStringImpl extends CString {

	public WStringImpl(int addr, int maxLen) {
		super(addr, maxLen);
	}

	@Override
	public void set(String str, int maxLen) {
		StringType.marshalWString(addr, str, maxLen);
	}

	@Override
	protected String resolve() {
		return StringType.unmarshalWString(addr, maxLen);
	}

	@Override
	public Storage getStorage() {
		return Storage.WCHAR;
	}

}
