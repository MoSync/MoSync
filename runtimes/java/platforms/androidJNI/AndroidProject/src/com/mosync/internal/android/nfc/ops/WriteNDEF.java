package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_WRITTEN;

import com.mosync.internal.android.nfc.INDEFMessageHolder;
import com.mosync.internal.android.nfc.INFCTag;
import com.mosync.internal.android.nfc.NDEFMessage;
import com.mosync.internal.android.nfc.NFCEvent;

public class WriteNDEF extends TagRunnable<INFCTag> {

	private NDEFMessage message;

	public WriteNDEF(INFCTag tag, NDEFMessage message) {
		super(tag, EVENT_TYPE_NFC_TAG_DATA_WRITTEN);
		this.message = message;
	}

	@Override
	protected NFCEvent doRun() throws Exception {
		((INDEFMessageHolder) tag).writeNDEFMessage(message);
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_WRITTEN, tag.getHandle(), 0, 0);
	}

}
