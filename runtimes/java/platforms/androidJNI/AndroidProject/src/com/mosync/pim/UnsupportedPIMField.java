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
