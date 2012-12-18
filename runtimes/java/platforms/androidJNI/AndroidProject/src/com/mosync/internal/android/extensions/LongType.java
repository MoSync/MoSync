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
		long result =((long) (data[offset + 7] & 0xff)) << 56 |
				((long) (data[offset + 6] & 0xff)) << 48 |
				((long) (data[offset + 5] & 0xff)) << 40 |
				((long) (data[offset + 4] & 0xff)) << 32 |
				((long) (data[offset + 3] & 0xff)) << 24 |
				((long) (data[offset + 2] & 0xff)) << 16 |
				((long) (data[offset + 1] & 0xff)) << 8 |
				((long) (data[offset] & 0xff));
		return result;
	}


}
