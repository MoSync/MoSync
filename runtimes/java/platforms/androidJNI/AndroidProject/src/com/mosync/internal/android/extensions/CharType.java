package com.mosync.internal.android.extensions;

public class CharType extends TypeDescriptor {

	private static CharType instance = new CharType();

	public static CharType getInstance() {
		return instance;
	}

	private CharType() { }

	@Override
	public Class getNativeClass() {
		return Byte.TYPE;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		return (byte) data[offset];
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		data[offset] = (Byte) o;
	}

	@Override
	public int size() {
		return 1;
	}

}
