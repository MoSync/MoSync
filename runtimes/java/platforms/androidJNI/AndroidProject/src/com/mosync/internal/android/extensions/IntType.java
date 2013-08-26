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



public class IntType extends TypeDescriptor {

	private static IntType instance = new IntType();

	public static IntType getInstance() {
		return instance;
	}

	private IntType() { }

	@Override
	public Class getNativeClass() {
		return Integer.TYPE;
	}

	public int size() {
		return 4;
	}

	public Object unmarshal(byte[] data, int offset) {
		return unmarshalInt(data, offset);
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		marshalInt((Integer) o, data, offset);
	}

	public static int unmarshalInt(byte[] data, int offset) {
		int result = ((data[offset + 3] & 0xff) << 24) | ((data[offset + 2] & 0xff) << 16)
				| ((data[offset + 1] & 0xff) << 8) | (data[offset] & 0xff);
		return result;
	}

	public static void marshalInt(int i, byte[] data, int offset) {
		data[offset] = (byte) (i & 0xff);
		data[offset + 1] = (byte) ((i >> 8) & 0xff);
		data[offset + 2] = (byte) ((i >> 16) & 0xff);
		data[offset + 3] = (byte) ((i >> 24) & 0xff);
	}

	public static int readIntFromMem(int addr) {
		return getMemorySlice(addr, 4).getInt();
	}

}