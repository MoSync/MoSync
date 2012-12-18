package com.mosync.internal.android.extensions;

import android.util.Log;

public class FloatType extends TypeDescriptor {

	@Override
	public Class getNativeClass() {
		return Float.TYPE;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		return Float.intBitsToFloat(IntType.unmarshalInt(data, offset));
	}

	@Override
	public int size() {
		return 4;
	}

}
