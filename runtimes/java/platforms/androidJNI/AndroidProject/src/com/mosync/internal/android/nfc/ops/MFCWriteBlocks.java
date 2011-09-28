package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_WRITTEN;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.MifareClassic;

import com.mosync.internal.android.nfc.MifareClassicTag;
import com.mosync.internal.android.nfc.NFCEvent;

public class MFCWriteBlocks extends TagRunnable<MifareClassicTag> {

	private static final int BLOCK_SIZE = 16;
	private final MifareClassic mifareTag;
	private final int firstBlock;
	private final ByteBuffer srcBuffer;

	public MFCWriteBlocks(MifareClassicTag mifareTag, int firstBlock, ByteBuffer srcBuffer) {
		super(mifareTag, EVENT_TYPE_NFC_TAG_DATA_WRITTEN);
		this.mifareTag = mifareTag.nativeTag();
		this.firstBlock = firstBlock;
		this.srcBuffer = srcBuffer;
	}

	@Override
	public NFCEvent doRun() throws IOException {
		int remaining = srcBuffer.limit();
		int blockIndex = firstBlock;
		byte[] block = new byte[BLOCK_SIZE];
		int writeCount = 0;
		int blockCount = 0;
		while (remaining > 0) {
			int length = Math.min(BLOCK_SIZE, srcBuffer.remaining());
			if (length < BLOCK_SIZE) {
				// Just zero it out.
				block = new byte[BLOCK_SIZE];
			}
			srcBuffer.get(block, 0, length);
			mifareTag.writeBlock(blockIndex + blockCount, block);
			writeCount += length;
			remaining -= length;
			blockCount++;
		}
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_WRITTEN, tag.getHandle(), writeCount, this.firstBlock);
	}

	@Override
	public String toString() {
		return String.format("Write %d bytes, starting at block %d of handle %d", srcBuffer.limit(), firstBlock, tag.getHandle());
	}
}
