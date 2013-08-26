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

import java.nio.ByteBuffer;
import java.util.HashSet;

import com.mosync.api.Marshaller;
import com.mosync.internal.android.MoSyncThread;

public abstract class TypeDescriptor implements Marshaller {

	private boolean out;

	public abstract Class getNativeClass();

	public abstract int size();

	public Object readFromMemory(int addr) {
		byte[] buf = new byte[size()];
		ByteBuffer mem = getMemorySlice(addr, size());
		mem.get(buf);
		return unmarshal(buf, 0);
	}

	public void writeToMemory(int addr, Object o) {
		byte[] buf = new byte[size()];
		ByteBuffer mem = getMemorySlice(addr, size());
		marshal(o, buf, 0);
		mem.put(buf);
	}

	/**
	 * Returns a memory slice with the correct endianness
	 * @param addr
	 * @param len
	 * @return
	 */
	protected static ByteBuffer getMemorySlice(int addr, int len) {
		return MoSyncThread.getInstance().getMemorySlice(addr, len).order(null);
	}


	public String toString() {
		return getNativeClass().getSimpleName();
	}

	public void setOut(boolean out) {
		this.out = out;
	}

	public boolean isOut() {
		return out;
	}

	public boolean isPrimitive() {
		return getNativeClass().isPrimitive();
	}

}