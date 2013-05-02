package com.mosync.pim;

import com.mosync.api.CString;
import com.mosync.api.CString.Storage;
import com.mosync.api.Pointer;
import com.mosync.internal.android.extensions.StringType;

public abstract class PIMStringField extends PIMField {

	byte[] getData(int index) {
		String val = getSpecificData(index);
		byte[] buffer = new byte[getDataSize(val)];
		StringType.marshalWString(buffer, 0, val, buffer.length);
		return buffer;
	}

	String getSpecificData(int index) {
		String[] val = mValues.get(index);
		return val[1];
	}

	int getDataSize(String val) {
		return CString.getCharSize(Storage.WCHAR) * (val.length() + 1);
	}

	void setData(int index, Pointer<Void> buffer) {
		String val = CString.fromByteArray(buffer, Storage.WCHAR, -1).toString();
		setSpecificData(val, index);
	}

	void setSpecificData(String data, int index) {
		String[] val = mValues.get(index);
		val[1] = data;
		mValues.set(index, val);
	}
}
