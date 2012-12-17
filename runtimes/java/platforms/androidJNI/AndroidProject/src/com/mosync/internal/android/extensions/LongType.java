package com.mosync.internal.android.extensions;

public class LongType extends TypeDescriptor {

	@Override
	public Class getNativeClass() {
		return Long.TYPE;
	}

	@Override
	public int size() {
		return 8;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		return unmarshalLong(data, offset);
	}

	public static long unmarshalLong(byte[] data, int offset) {
		return (data[offset + 7] << 56) | (data[offset + 6] << 48)
				| (data[offset + 5] << 40) | (data[offset + 4] << 32)
				| (data[offset + 3] << 24) | (data[offset + 2] << 16)
				| (data[offset + 1] << 8) | data[offset];
	}


}
