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

import java.nio.ByteBuffer;
import java.util.Map;

import com.mosync.api.CString;
import com.mosync.api.Pointer;
import com.mosync.api.CString.Storage;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.extensions.IntType;
import com.mosync.internal.android.extensions.StringType;
import com.mosync.internal.android.extensions.WStringImpl;

public class PIMUtil {

	public static MoSyncThread sMoSyncThread;

	/**
	 * Gets the key from a map by it's value.
	 * @param hm
	 * @param value
	 * @return
	 */
	static Object getKeyFromValue(Map<?, ?> hm, Object value) {
		for (Object o : hm.keySet()) {
			if (hm.get(o).equals(value)) {
				return o;
			}
		}
		return null;
	}

	/**
	 * @return The MoSync data section memory buffer.
	 */
	static public MoSyncThread getThread() {
		return sMoSyncThread;
	}

	static void writeStringArray(String[] value, byte[] buffer) {
		IntType.marshalInt(value.length, buffer, 0);
		int offset = IntType.getInstance().size();
		for (int i = 0; i < value.length; i++) {
			offset += StringType.marshalWString(buffer, offset, value[i], value[i].length() + 1);
		}
	}

	static String[] readStringArray(Pointer<Void> buffer) {
		int addr = buffer.getAddress();
		int length = IntType.readIntFromMem(addr);
		buffer = buffer.offset(IntType.getInstance().size());
		String[] val = new String[length];
		for (int i = 0; i < length; i++) {
			CString str = CString.fromByteArray(buffer, Storage.WCHAR, -1);
			val[i] = str.toString();
			buffer = str.getEnd();
		}

		return val;
	}

}
