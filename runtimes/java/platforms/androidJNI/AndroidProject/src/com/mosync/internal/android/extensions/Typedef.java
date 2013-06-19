package com.mosync.internal.android.extensions;

import java.util.HashSet;

public class Typedef extends TypeDescriptor {

	private String name;
	private String typeref;
	private ExtensionModule module;

	public Typedef(ExtensionModule module, String name, String typeref) {
		this.module = module;
		this.name = name;
		this.typeref = typeref;
	}

	public String getTypeName() {
		return name;
	}

	public TypeDescriptor resolve() {
		return resolve(new HashSet<String>());
	}

	private TypeDescriptor resolve(HashSet<String> circularCheck) {
		TypeDescriptor ref = module.getTypeDescriptor(typeref, 0, false);
		if (ref == null) {
			throw new IllegalArgumentException("Unknown type: " + typeref);
		}
		if (circularCheck.contains(name)) {
			throw new IllegalArgumentException("Circular definition of " + name);
		}
		circularCheck.add(name);
		if (ref instanceof Typedef) {
			return ((Typedef) ref).resolve(circularCheck);
		}
		return ref;
	}

	@Override
	public Class getNativeClass() {
		return resolve().getNativeClass();
	}

	@Override
	public int size() {
		return resolve().size();
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		throw new UnsupportedOperationException("Typedefs must be resolved!");
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		throw new UnsupportedOperationException("Typedefs must be resolved!");
	}

	public String toString() {
		return "name = " + resolve().toString();
	}


}