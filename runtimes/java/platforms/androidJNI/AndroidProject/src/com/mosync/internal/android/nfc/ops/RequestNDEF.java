package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_READ;

import com.mosync.internal.android.nfc.INDEFMessageHolder;
import com.mosync.internal.android.nfc.INFCTag;
import com.mosync.internal.android.nfc.NFCEvent;
import com.mosync.internal.android.nfc.ResourcePool;

public class RequestNDEF extends TagRunnable<INFCTag> {

	private final ResourcePool pool;

	public RequestNDEF(ResourcePool pool, INFCTag tag) {
		super(tag, EVENT_TYPE_NFC_TAG_DATA_READ);
		this.pool = pool;
	}

	@Override
	protected NFCEvent doRun() throws Exception {
		((INDEFMessageHolder) tag).requestNDEFMessage(pool);
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, tag.getHandle(), 0, 0);
	}

}
