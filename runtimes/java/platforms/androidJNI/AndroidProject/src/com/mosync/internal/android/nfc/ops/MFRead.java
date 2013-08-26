/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package com.mosync.internal.android.nfc.ops;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_DATA_READ;

import java.io.IOException;
import java.nio.ByteBuffer;

import com.mosync.internal.android.nfc.INFCTag;
import com.mosync.internal.android.nfc.ISizeHolder;
import com.mosync.internal.android.nfc.NFCEvent;

public abstract class MFRead<TagType extends INFCTag> extends TagRunnable<TagType> {

	private final int firstPage;
	private final ByteBuffer dstBuffer;

	public MFRead(TagType tag, int firstPage,
			ByteBuffer dstBuffer) {
		super(tag, EVENT_TYPE_NFC_TAG_DATA_READ);
		this.firstPage = firstPage;
		this.dstBuffer = dstBuffer;
	}

	@Override
	public NFCEvent doRun() throws IOException {
		int size = ((ISizeHolder) tag).getSize();
		int resultSize = size > 0 ? Math.min(size, dstBuffer.limit()) : dstBuffer.limit();
		int chunkOffset = firstPage;
		byte[] result = new byte[resultSize];
		int readCount = 0;
		int chunkCount = 0;
		while (resultSize > 0) {
			byte[] readChunk = readChunk((chunkOffset + chunkCount) % size);
			int length = Math.min(resultSize, readChunk.length);
			System.arraycopy(readChunk, 0, result, readCount, length);
			readCount += length;
			resultSize -= length;
			chunkCount++;
		}

		dstBuffer.put(result);
		return new NFCEvent(EVENT_TYPE_NFC_TAG_DATA_READ, tag.getHandle(), readCount, firstPage);
	}

	protected abstract byte[] readChunk(int chunkIndex) throws IOException;

	@Override
	public String toString() {
		return String.format("Read %d bytes, starting at page %d of handle %d", dstBuffer.limit(), firstPage, tag.getHandle());
	}
}
