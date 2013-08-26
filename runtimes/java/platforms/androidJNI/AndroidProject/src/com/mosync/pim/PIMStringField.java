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

package com.mosync.pim;

import com.mosync.api.CString;
import com.mosync.api.CString.Storage;
import com.mosync.api.Pointer;
import com.mosync.internal.android.extensions.StringType;

public abstract class PIMStringField extends PIMField {

	byte[] getData(int index) {
		String val = getSpecificData(index);
		byte[] buffer = new byte[getDataSize(val)];
		StringType.marshalWString(buffer, 0, val, buffer.length);
		return buffer;
	}

	String getSpecificData(int index) {
		String[] val = mValues.get(index);
		return val[1];
	}

	int getDataSize(String val) {
		return CString.getCharSize(Storage.WCHAR) * (val.length() + 1);
	}

	void setData(int index, Pointer<Void> buffer) {
		String val = CString.fromByteArray(buffer, Storage.WCHAR, -1).toString();
		setSpecificData(val, index);
	}

	void setSpecificData(String data, int index) {
		String[] val = mValues.get(index);
		val[1] = data;
		mValues.set(index, val);
	}
}
