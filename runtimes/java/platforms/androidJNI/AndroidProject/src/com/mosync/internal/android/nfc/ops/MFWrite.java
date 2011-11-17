package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_WRITTEN;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.MifareUltralight;

import com.mosync.internal.android.nfc.INFCTag;
import com.mosync.internal.android.nfc.MifareUltralightTag;
import com.mosync.internal.android.nfc.NFCEvent;

public abstract class MFWrite<TagType extends INFCTag> extends TagRunnable<TagType> {

	private final int firstPage;
	private final ByteBuffer srcBuffer;

	public MFWrite(TagType tag, int firstPage,
			ByteBuffer srcBuffer) {
		super(tag, EVENT_TYPE_NFC_TAG_DATA_WRITTEN);
		this.firstPage = firstPage;
		this.srcBuffer = srcBuffer;
	}

	@Override
	public NFCEvent doRun() throws IOException {
		int chunkSize = chunkSize();
		int remaining = srcBuffer.limit();
		int chunkOffset = firstPage;
		byte[] chunk = new byte[chunkSize];
		int writeCount = 0;
		int chunkCount = 0;
		while (remaining > 0) {
			int length = Math.min(chunkSize, remaining);
			if (length < chunkSize) {
				// Just zero it out.
				chunk = new byte[chunkSize];
			}
			srcBuffer.get(chunk, 0, length);
			writeChunk(chunkOffset + chunkCount, chunk);
			writeCount += length;
			remaining -= length;
			chunkCount++;
		}

		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_WRITTEN, tag.getHandle(), writeCount, firstPage);
	}

	protected abstract void writeChunk(int chunkIndex, byte[] chunk) throws IOException;

	protected abstract int chunkSize();

	@Override
	public String toString() {
		return String.format("Write %d bytes, starting at page %d of handle %d", srcBuffer.limit(), firstPage, tag.getHandle());
	}
}
