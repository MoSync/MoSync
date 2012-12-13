package com.mosync.internal.android.extensions;

import java.util.HashSet;

public abstract class TypeDescriptor {
	public abstract Class getNativeClass();

	public abstract Object convert(int[] args, int offset);

	public int size() {
		return 1;
	}

	public boolean isArray() {
		return false;
	}

	public final TypeDescriptor resolve() {
		return resolve(new HashSet<String>());
	}

	public TypeDescriptor resolve(HashSet<String> circularCheck) {
		return this;
	}

	public String toString() {
		return getNativeClass().getSimpleName();
	}
}