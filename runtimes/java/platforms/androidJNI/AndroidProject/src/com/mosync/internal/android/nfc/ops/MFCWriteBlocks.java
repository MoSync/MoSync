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

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.MifareClassic;

import com.mosync.internal.android.nfc.MifareClassicTag;

public class MFCWriteBlocks extends MFWrite<MifareClassicTag> {

	private static final int BLOCK_SIZE = 16;
	private final MifareClassic mifareTag;

	public MFCWriteBlocks(MifareClassicTag mifareTag, int firstBlock, ByteBuffer srcBuffer) {
		super(mifareTag, firstBlock, srcBuffer);
		this.mifareTag = mifareTag.nativeTag();
	}

	@Override
	protected void writeChunk(int chunkIndex, byte[] chunk) throws IOException {
		mifareTag.writeBlock(chunkIndex, chunk);
	}

	@Override
	protected int chunkSize() {
		return BLOCK_SIZE;
	}

}
