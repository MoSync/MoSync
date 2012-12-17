package com.mosync.internal.android.extensions;

public class DoubleType extends TypeDescriptor {

	@Override
	public Class getNativeClass() {
		return Double.TYPE;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		return Double.longBitsToDouble(LongType.unmarshalLong(data, offset));
	}

	@Override
	public int size() {
		return 8;
	}

}
