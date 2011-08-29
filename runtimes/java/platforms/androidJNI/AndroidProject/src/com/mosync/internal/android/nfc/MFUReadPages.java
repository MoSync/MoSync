package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_READ;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_CONNECTION_LOST;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_IO_ERROR;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.TagLostException;
import android.nfc.tech.MifareUltralight;

public class MFUReadPages implements RunnableWithResult<NFCEvent> {

	private final int handle;
	private final MifareUltralight tag;
	private final int firstPage;
	private final ByteBuffer dstBuffer;

	public MFUReadPages(int handle, MifareUltralight tag, int firstPage,
			ByteBuffer dstBuffer) {
		this.handle = handle;
		this.tag = tag;
		this.firstPage = firstPage;
		this.dstBuffer = dstBuffer;
	}

	@Override
	public NFCEvent run() {
		int resultSize = dstBuffer.limit();
		int pageOffset = firstPage;
		byte[] result = new byte[resultSize];
		int readCount = 0;
		int pageCount = 0;
		while (resultSize > 0) {
			try {
				byte[] readPages = tag.readPages(pageOffset + pageCount);
				int length = Math.min(resultSize, readPages.length);
				System.arraycopy(readPages, 0, result, readCount, length);
				readCount += length;
				resultSize -= length;
				pageCount++;
			} catch (TagLostException e) {
				return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, handle, MA_NFC_TAG_CONNECTION_LOST, -1);
			} catch (Exception e) {
				return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, handle, MA_NFC_TAG_IO_ERROR, -1);
			}
		}

		dstBuffer.put(result);
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, handle, readCount, firstPage);
	}

	@Override
	public String toString() {
		return String.format("Read %d bytes, starting at page %d of handle %d", dstBuffer.limit(), firstPage, handle);
	}
}
