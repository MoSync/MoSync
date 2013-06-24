package com.mosync.internal.android.extensions;

public class VoidType extends TypeDescriptor {

	private static VoidType instance = new VoidType();

	public static VoidType getInstance() {
		return instance;
	}

	VoidType() {

	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		return null;
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
	}

	@Override
	public Class getNativeClass() {
		return Void.TYPE;
	}

	@Override
	public int size() {
		return 0;
	}

}
