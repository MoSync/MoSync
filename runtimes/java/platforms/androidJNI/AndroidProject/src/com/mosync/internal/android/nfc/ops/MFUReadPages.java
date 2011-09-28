package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_READ;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.MifareUltralight;

import com.mosync.internal.android.nfc.MifareUltralightTag;
import com.mosync.internal.android.nfc.NFCEvent;

public class MFUReadPages extends TagRunnable<MifareUltralightTag> {

	private final MifareUltralight mifareTag;
	private final int firstPage;
	private final ByteBuffer dstBuffer;

	public MFUReadPages(MifareUltralightTag tag, int firstPage,
			ByteBuffer dstBuffer) {
		super(tag, EVENT_TYPE_NFC_TAG_DATA_READ);
		this.mifareTag = tag.nativeTag();
		this.firstPage = firstPage;
		this.dstBuffer = dstBuffer;
	}

	@Override
	public NFCEvent doRun() throws IOException {
		int resultSize = dstBuffer.limit();
		int pageOffset = firstPage;
		byte[] result = new byte[resultSize];
		int readCount = 0;
		int pageCount = 0;
		while (resultSize > 0) {
			byte[] readPages = mifareTag.readPages(pageOffset + pageCount);
			int length = Math.min(resultSize, readPages.length);
			System.arraycopy(readPages, 0, result, readCount, length);
			readCount += length;
			resultSize -= length;
			pageCount++;
		}

		dstBuffer.put(result);
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, tag.getHandle(), readCount, firstPage);
	}

	@Override
	public String toString() {
		return String.format("Read %d bytes, starting at page %d of handle %d", dstBuffer.limit(), firstPage, tag.getHandle());
	}
}
