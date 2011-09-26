package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_READ_ONLY;

import com.mosync.internal.android.nfc.INFCTag;
import com.mosync.internal.android.nfc.IReadOnlySupport;
import com.mosync.internal.android.nfc.NFCEvent;

public class SetReadOnly extends TagRunnable<INFCTag> {

	public SetReadOnly(INFCTag tag) {
		super(tag, EVENT_TYPE_NFC_TAG_READ_ONLY);
	}

	@Override
	protected NFCEvent doRun() throws Exception {
		IReadOnlySupport readOnlySupport = (IReadOnlySupport) tag;
		readOnlySupport.setReadOnly();
		return new NFCEvent(EVENT_TYPE_NFC_TAG_READ_ONLY, tag.getHandle(), 0, 0);
	}

}
