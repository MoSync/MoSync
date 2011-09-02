package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_READ;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_CONNECTION_LOST;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_IO_ERROR;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.TagLostException;
import android.nfc.tech.MifareClassic;

public class MFCReadBlocks extends TagRunnable<MifareClassicTag> {

	private final MifareClassic mifareTag;
	private final int firstBlock;
	private final ByteBuffer dstBuffer;

	public MFCReadBlocks(MifareClassicTag mifareTag, int firstBlock,
			ByteBuffer dstBuffer) {
		super(mifareTag, EVENT_TYPE_NFC_TAG_DATA_READ);
		this.mifareTag = mifareTag.nativeTag();
		this.firstBlock = firstBlock;
		this.dstBuffer = dstBuffer;
	}

	@Override
	public NFCEvent doRun() throws IOException {
		int resultSize = dstBuffer.capacity();
		int blockIndex = firstBlock;
		byte[] result = new byte[resultSize];
		int readCount = 0;
		int blockCount = 0;
		while (resultSize > 0) {
			byte[] readBlock = mifareTag.readBlock(blockIndex + blockCount);
			int length = Math.min(resultSize, readBlock.length);
			System.arraycopy(readBlock, 0, result, readCount, length);
			readCount += length;
			resultSize -= length;
			blockCount++;
		}
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, tag.getHandle(), readCount, this.firstBlock);
	}

	@Override
	public String toString() {
		return String.format("Read %d bytes, starting at block %d of handle %d", dstBuffer.limit(), firstBlock, tag.getHandle());
	}
}
