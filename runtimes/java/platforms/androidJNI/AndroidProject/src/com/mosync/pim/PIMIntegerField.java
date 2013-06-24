package com.mosync.pim;

import com.mosync.api.Pointer;
import com.mosync.internal.android.extensions.IntType;

public abstract class PIMIntegerField extends PIMField {

	byte[] getData(int index) {
		String val = getSpecificData(index);
		byte[] buffer = new byte[IntType.getInstance().size()];
		IntType.marshalInt(Integer.parseInt(val), buffer, 0);
		return buffer;
	}

	String getSpecificData(int index) {
		String[] val = mValues.get(index);
		return val[1];
	}

	int getDataSize(String val) {
		return (Integer.SIZE / 8);
	}

	void setData(int index, Pointer<Void> buffer) {
		String val = Integer.toString(IntType.readIntFromMem(buffer.getAddress()));
		setSpecificData(val, index);
	}

	void setSpecificData(String data, int index) {
		String[] val = mValues.get(index);
		val[1] = data;
		mValues.set(index, val);
	}

}
