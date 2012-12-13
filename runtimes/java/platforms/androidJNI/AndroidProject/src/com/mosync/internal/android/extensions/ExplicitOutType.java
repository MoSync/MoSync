package com.mosync.internal.android.extensions;

import java.lang.reflect.Array;

public class ExplicitOutType extends TypeDescriptor {
	private TypeDescriptor delegate;
	private Class arrayClass = null;

	public ExplicitOutType(TypeDescriptor delegate) {
		this.delegate = delegate;
	}

	@Override
	public Class getNativeClass() {
		if (arrayClass == null) {
			Object array = Array.newInstance(delegate.getNativeClass(), 0);
			arrayClass = array.getClass();
		}
		return arrayClass;
	}

	@Override
	public Object convert(int[] args, int offset) {
		Object result = delegate.convert(args, offset);
		Object resultArray = Array.newInstance(delegate.getNativeClass(), 1);
		Array.set(resultArray, 0, result);
		return resultArray;
	}
}