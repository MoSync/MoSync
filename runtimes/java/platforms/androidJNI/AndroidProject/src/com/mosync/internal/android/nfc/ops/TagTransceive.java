package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_READ;

import java.io.IOException;
import java.nio.ByteBuffer;

import com.mosync.internal.android.nfc.ITransceivable;
import com.mosync.internal.android.nfc.NFCEvent;

public class TagTransceive extends TagRunnable<ITransceivable> {

	private final ByteBuffer src;
	private final ByteBuffer dst;
	private final int dstPtr;

	public TagTransceive(ITransceivable tag, ByteBuffer src, ByteBuffer dst, int dstPtr) {
		super(tag, EVENT_TYPE_NFC_TAG_DATA_READ);
		this.src = src;
		this.dst = dst;
		this.dstPtr = dstPtr;
	}

	@Override
	public NFCEvent doRun() throws IOException {
		byte[] buffer = new byte[src.limit()];
		src.get(buffer);
		byte[] result = tag.transceive(buffer);
		dst.put(result);
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, tag.getHandle(), result.length, dstPtr);
	}

}
