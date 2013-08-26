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

public class PointerImpl extends Pointer {

	private int addr;
	private TypeDescriptor delegate;

	public PointerImpl(TypeDescriptor delegate, int addr) {
		this.delegate = delegate;
		this.addr = addr;
	}

	@Override
	public Object deref() {
		if (addr == 0) {
			return null;
		}
		return at(0);
	}

	@Override
	public Object at(int ix) {
		int size = delegate.size();
		return delegate.readFromMemory(addr + ix * size);
	}

	public int getAddress() {
		return addr;
	}

	@Override
	public void set(Object o) {
		set(o, 0);
	}

	@Override
	public void set(Object o, int ix) {
		int size = delegate.size();
		delegate.writeToMemory(addr + ix * size, o);
	}

	public boolean isNull() {
		return addr == 0;
	}

	public Pointer<Void> offset(int bytes) {
		if (isNull()) {
			throw new NullPointerException();
		}
		return new PointerImpl(VoidType.getInstance(), addr + bytes);
	}

	public String toString() {
		return "(" + delegate.toString() + "*): @0x" + Integer.toHexString(addr);
	}

}