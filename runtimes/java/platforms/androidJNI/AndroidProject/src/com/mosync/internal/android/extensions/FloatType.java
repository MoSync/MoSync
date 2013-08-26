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
