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
