package com.mosync.internal.android.extensions;

public class LongType extends TypeDescriptor {

	private static LongType instance = new LongType();

	public static LongType getInstance() {
		return instance;
	}

	private LongType() { }

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

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		marshalLong((Long) o, data, offset);
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

	public static void marshalLong(long l, byte[] data, int offset) {
		data[offset] = (byte) (l & 0xff);
		data[offset + 1] = (byte) ((l >> 8) & 0xff);
		data[offset + 2] = (byte) ((l >> 16) & 0xff);
		data[offset + 3] = (byte) ((l >> 24) & 0xff);
		data[offset + 4] = (byte) ((l >> 32) & 0xff);
		data[offset + 5] = (byte) ((l >> 40) & 0xff);
		data[offset + 6] = (byte) ((l >> 48) & 0xff);
		data[offset + 7] = (byte) ((l >> 56) & 0xff);
	}



}
