package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_READ;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_CONNECTION_LOST;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_IO_ERROR;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.TagLostException;
import android.nfc.tech.MifareClassic;

public class MFCReadBlocks implements RunnableWithResult<NFCEvent> {

	private final MifareClassic mifareTag;
	private final int handle;
	private final int firstBlock;
	private final ByteBuffer dstBuffer;

	public MFCReadBlocks(int handle, MifareClassic mifareTag, int firstBlock,
			ByteBuffer dstBuffer) {
		this.handle = handle;
		this.mifareTag = mifareTag;
		this.firstBlock = firstBlock;
		this.dstBuffer = dstBuffer;
	}

	@Override
	public NFCEvent run() {
		int resultSize = dstBuffer.capacity();
		int blockIndex = firstBlock;
		byte[] result = new byte[resultSize];
		int readCount = 0;
		int blockCount = 0;
		while (resultSize > 0) {
			try {
				byte[] readBlock = mifareTag.readBlock(blockIndex + blockCount);
				int length = Math.min(resultSize, readBlock.length);
				System.arraycopy(readBlock, 0, result, readCount, length);
				readCount += length;
				resultSize -= length;
				blockCount++;
			} catch (TagLostException e) {
				return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, handle, MA_NFC_TAG_CONNECTION_LOST, -1);
			} catch (Exception e) {
				return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, handle, MA_NFC_TAG_IO_ERROR, -1);
			}
		}
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, handle, readCount, this.firstBlock);
	}

	@Override
	public String toString() {
		return String.format("Read %d bytes, starting at block %d of handle %d", dstBuffer.limit(), firstBlock, handle);
	}
}
