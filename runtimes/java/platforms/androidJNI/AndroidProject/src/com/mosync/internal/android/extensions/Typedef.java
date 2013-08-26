/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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