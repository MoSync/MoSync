package com.mosync.internal.android.extensions;

public class DoubleType extends TypeDescriptor {

	private static DoubleType instance = new DoubleType();

	public static DoubleType getInstance() {
		return instance;
	}

	private DoubleType() { }

	@Override
	public Class getNativeClass() {
		return Double.TYPE;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		double d1 = Double.longBitsToDouble(LongType.unmarshalLong(data, offset));
		return d1;
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		Double d = (Double) o;
		LongType.marshalLong(Double.doubleToRawLongBits(d), data, offset);
	}

	@Override
	public int size() {
		return 8;
	}

}
