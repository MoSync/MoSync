package com.mosync.internal.android.extensions;

public class IntType extends TypeDescriptor {

	@Override
	public Class getNativeClass() {
		return Integer.TYPE;
	}

	@Override
	public Object convert(int[] args, int offset) {
		return args[offset];
	}
}