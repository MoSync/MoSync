package com.mosync.internal.android.extensions;



public class IntType extends TypeDescriptor {

	@Override
	public Class getNativeClass() {
		return Integer.TYPE;
	}

	public int size() {
		return 4;
	}

	public Object unmarshal(byte[] data, int offset) {
		return unmarshalInt(data, offset);
	}

	public static int unmarshalInt(byte[] data, int offset) {
		int result = ((data[offset + 3] & 0xff) << 24) | ((data[offset + 2] & 0xff) << 16)
				| ((data[offset + 1] & 0xff) << 8) | (data[offset] & 0xff);
		return result;
	}

	public static int readIntFromMem(int addr) {
		return getMemorySlice(addr, 4).getInt();
	}
}