package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_READ;

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
