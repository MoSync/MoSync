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

package com.mosync.api;

import com.mosync.internal.android.extensions.CStringImpl;
import com.mosync.internal.android.extensions.PointerImpl;
import com.mosync.internal.android.extensions.StringType;
import com.mosync.internal.android.extensions.VoidType;
import com.mosync.internal.android.extensions.WStringImpl;


/**
 * A class representing a C string that can be shared
 * between MoSync memory and an Android extension
 * @author Mattias Bybro
 *
 */
public abstract class CString {

	public static enum Storage {
		CHAR, WCHAR
	}


	protected int addr;
	protected int maxLen;
	protected boolean resolved = false;
	protected String str;
	private Pointer<Void> endPtr;

	/**
	 * Creates a zero-terminated {@code CString} from a pointer.
	 * @param ptr The pointer being used
	 * @param storage the type of storage used
	 * @param maxLen The maximum length of the string, or {@code -1} if undefined
	 * @return
	 */
	public static CString fromByteArray(Pointer<Void> ptr, Storage storage, int maxLen) {
		return storage == Storage.CHAR ?
			new CStringImpl(ptr.getAddress(), maxLen) :
			new WStringImpl(ptr.getAddress(), maxLen);
	}

	public static int getCharSize(Storage storage) {
		return StringType.getCharSize(storage);
	}

	protected CString(int addr, int maxLen) {
		this.addr = addr;
		this.maxLen = maxLen;
	}

	public Pointer<Void> getPointer() {
		return new PointerImpl(VoidType.getInstance(), addr);
	}

	public Pointer<Void> getEnd() {
		internalResolve();
		return endPtr;
	}

	public String toString() {
		internalResolve();
		return str;
	}

	private void internalResolve() {
		if (!resolved) {
			str = resolve();
			int lenInBytes = str == null ? 0 : (str.length() + 1) * getCharSize(getStorage());
			endPtr = new PointerImpl(VoidType.getInstance(), addr + lenInBytes);
			resolved = true;
		}
	}

	public abstract void set(String str, int maxLen);

	public abstract Storage getStorage();

	protected abstract String resolve();

}
