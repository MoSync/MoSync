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

import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_ISO_DEP;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_CL;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_UL;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NDEF;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NDEF_FORMATTABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NFC_A;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NFC_B;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.HashMap;

import android.nfc.Tag;
import android.nfc.tech.TagTechnology;

public class GenericTag extends ResourceBase implements INFCTag {

	private HashMap<Integer, INFCTag> typedTags = new HashMap<Integer, INFCTag>();
	private final Tag tag;

	public GenericTag(ResourcePool pool, Tag tag) {
		super(pool);
		this.tag = tag;
	}

	public Tag getTag() {
		return tag;
	}

	public boolean isType(int type) {
		return createTypedTag(ResourcePool.NULL, type) != null;
	}

	@Override
	public INFCTag getTypedTag(ResourcePool pool, int type) {
		INFCTag typedTag = typedTags.get(type);
		if (typedTag == null || typedTag.isDestroyed()) {
			typedTag = createTypedTag(pool, type);
			typedTags.put(type, typedTag);
		}
		return typedTag;
	}

	private INFCTag createTypedTag(ResourcePool pool, int type) {
		switch (type) {
		case MA_NFC_TAG_TYPE_NDEF:
			return NdefTag.get(pool, this);
		case MA_NFC_TAG_TYPE_MIFARE_CL:
			return MifareClassicTag.get(pool, this);
		case MA_NFC_TAG_TYPE_MIFARE_UL:
			return MifareUltralightTag.get(pool, this);
		case MA_NFC_TAG_TYPE_NFC_A:
			return NfcATag.get(pool, this);
		case MA_NFC_TAG_TYPE_NFC_B:
			return NfcBTag.get(pool, this);
		case MA_NFC_TAG_TYPE_ISO_DEP:
			return IsoDepTag.get(pool, this);
		case MA_NFC_TAG_TYPE_NDEF_FORMATTABLE:
			return NdefFormatableTag.get(pool, this);
		default:
			return null;
		}
	}

	@Override
	public void destroy(ResourcePool pool) {
		super.destroy(pool);
		for (INFCTag typedTag : typedTags.values()) {
			if (typedTag != null) {
				typedTag.destroy(pool);
			}
		}
		typedTags.clear();
	}

	@Override
	public void close() throws IOException {
		// Does nothing
	}

	@Override
	public void connect() throws IOException {
		// Does nothing
	}

	@Override
	public TagTechnology nativeTag() {
		return null;
	}

	@Override
	public int getId(ByteBuffer result) {
		// Does nothing
		return 0;
	}

}
