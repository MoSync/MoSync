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

import com.mosync.api.Pointer;

public class PointerType extends TypeDescriptor {
	TypeDescriptor delegate;

	public PointerType(TypeDescriptor delegate) {
		this.delegate = delegate;
	}

	@Override
	public Class getNativeClass() {
		return Pointer.class;
	}

	@Override
	public Object unmarshal(byte[] data, int offset) {
		return new PointerImpl(delegate, IntType.unmarshalInt(data, offset));
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		PointerImpl ptr = (PointerImpl) o;
		int addr = ptr.getAddress();
		IntType.marshalInt(addr, data, offset);
	}

	@Override
	public int size() {
		return 4;
	}

	public String toString() {
		return delegate.toString() + "*";
	}

}