package com.mosync.internal.android.extensions;

public class CharType extends TypeDescriptor {

	@Override
	public Class getNativeClass() {
		return Character.TYPE;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		return (char) data[offset];
	}

	@Override
	public int size() {
		return 1;
	}

}
