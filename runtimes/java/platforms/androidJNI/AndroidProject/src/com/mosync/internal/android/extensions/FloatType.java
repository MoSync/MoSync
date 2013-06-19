package com.mosync.internal.android.extensions;

public class FloatType extends TypeDescriptor {

	private static FloatType instance = new FloatType();

	public static FloatType getInstance() {
		return instance;
	}

	private FloatType() { }

	@Override
	public Class getNativeClass() {
		return Float.TYPE;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		return Float.intBitsToFloat(IntType.unmarshalInt(data, offset));
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		Float f = (Float) o;
		IntType.marshalInt(Float.floatToRawIntBits(f), data, offset);
	}

	@Override
	public int size() {
		return 4;
	}

}
