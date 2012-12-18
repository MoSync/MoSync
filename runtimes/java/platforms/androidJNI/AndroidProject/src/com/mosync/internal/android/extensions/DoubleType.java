package com.mosync.internal.android.extensions;

import android.util.Log;

public class DoubleType extends TypeDescriptor {

	@Override
	public Class getNativeClass() {
		return Double.TYPE;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		double d1 = Double.longBitsToDouble(LongType.unmarshalLong(data, offset));
		return d1;
	}

	@Override
	public int size() {
		return 8;
	}

}
