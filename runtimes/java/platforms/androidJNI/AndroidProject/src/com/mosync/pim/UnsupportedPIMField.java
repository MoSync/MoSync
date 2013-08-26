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

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import android.content.ContentResolver;

import com.mosync.api.Pointer;

public abstract class UnsupportedPIMField extends PIMField {

	void createMaps() {

	}

	void read(ContentResolver cr, String contactId) {
		print();
	}

	int checkForPreferredAttribute(int index) {
		return 0;
	}

	int getAndroidAttribute(int index) {
		return 0;
	}

	int setAttribute(int index, int attribute) {
		return MA_PIM_ERR_NONE;
	}

	char[] getLabel(int index) {
		return null;
	}

	void setLabel(int index, String label) {
	}

	boolean hasCustomLabel(int index) {
		return false;
	}

	byte[] getData(int index) {
		return null;
	}

	void setData(int index, Pointer<Void> buffer) {
	}

	boolean isSupported() {
		return false;
	}
}
