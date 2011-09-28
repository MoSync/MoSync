package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_WRITTEN;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.MifareUltralight;

import com.mosync.internal.android.nfc.MifareUltralightTag;
import com.mosync.internal.android.nfc.NFCEvent;

public class MFUWritePages extends TagRunnable<MifareUltralightTag> {

	private static final int PAGE_SIZE = 4;

	private final MifareUltralight mifareTag;
	private final int firstPage;
	private final ByteBuffer srcBuffer;

	public MFUWritePages(MifareUltralightTag tag, int firstPage,
			ByteBuffer srcBuffer) {
		super(tag, EVENT_TYPE_NFC_TAG_DATA_WRITTEN);
		this.mifareTag = tag.nativeTag();
		this.firstPage = firstPage;
		this.srcBuffer = srcBuffer;
	}

	@Override
	public NFCEvent doRun() throws IOException {
		int remaining = srcBuffer.limit();
		int pageOffset = firstPage;
		byte[] page = new byte[PAGE_SIZE];
		int writeCount = 0;
		int pageCount = 0;
		while (remaining > 0) {
			int length = Math.min(PAGE_SIZE, srcBuffer.remaining());
			if (length < PAGE_SIZE) {
				// Just zero it out.
				page = new byte[PAGE_SIZE];
			}
			mifareTag.writePage(pageOffset + pageCount, page);
			writeCount += length;
			remaining -= length;
			pageCount++;
		}

		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_WRITTEN, tag.getHandle(), writeCount, firstPage);
	}

	@Override
	public String toString() {
		return String.format("Read %d bytes, starting at page %d of handle %d", srcBuffer.limit(), firstPage, tag.getHandle());
	}
}
