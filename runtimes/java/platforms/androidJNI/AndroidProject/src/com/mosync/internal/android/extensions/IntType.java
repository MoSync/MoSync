package com.mosync.internal.android.extensions;

import java.nio.charset.UnmappableCharacterException;


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
		return (data[offset + 3] << 24) | (data[offset + 2] << 16)
				| (data[offset + 1] << 8) | data[offset];
	}
}