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

package com.mosync.internal.android.nfc;

import java.io.IOException;
import java.nio.ByteBuffer;

import android.nfc.tech.TagTechnology;

public abstract class NFCTagBase<NativeTagType extends TagTechnology> extends ResourceBase implements INFCTag<NativeTagType> {

	protected NativeTagType nativeTag;
	private final int typeId;

	public NFCTagBase(ResourcePool pool, NativeTagType nativeTag, int typeId) {
		super(pool);
		this.nativeTag = nativeTag;
		this.typeId = typeId;
	}

	@Override
	public void close() throws IOException {
		nativeTag.close();
	}

	@Override
	public void connect() throws IOException {
		nativeTag.connect();
	}

	@Override
	public INFCTag getTypedTag(ResourcePool pool, int type) {
		if (type == typeId) {
			return this;
		}
		return null;
	}

	@Override
	public int getId(ByteBuffer result) {
		byte[] id = nativeTag.getTag().getId();
		if (result == null) {
			return id.length;
		}
		int size = Math.min(result.remaining(), id.length);
		result.put(id, 0, size);
		return size;
	}

	@Override
	public NativeTagType nativeTag() {
		return nativeTag;
	}

}
