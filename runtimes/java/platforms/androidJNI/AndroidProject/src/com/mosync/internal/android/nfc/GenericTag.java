package com.mosync.internal.android.nfc;

import android.nfc.NdefMessage;
import android.nfc.Tag;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.MifareUltralight;
import android.nfc.tech.Ndef;


public class GenericTag extends ResourceBase implements INFCTag {

	private final Tag tag;
	private NDEFMessage ndef;
	private MifareClassic mfc;
	private MifareUltralight mfu;

	public GenericTag(ResourcePool pool, Tag tag) {
		super(pool);
		this.tag = tag;
	}

	@Override
	public void close() {
		// TODO Auto-generated method stub

	}

	@Override
	public void connect() {
		// TODO Auto-generated method stub

	}

	@Override
	public void destroy(ResourcePool pool) {
		if (ndef != null) {
			ndef.destroy(pool);
		}
		super.destroy(pool);
	}

	@Override
	public NDEFMessage getNDEFMessage(ResourcePool pool) {
		if (ndef != null) {
			return ndef;
		}

		Ndef ndef = Ndef.get(tag);
		if (ndef != null) {
			NdefMessage ndefMessage = ndef.getCachedNdefMessage();
			this.ndef = new NDEFMessage(pool, ndefMessage);
		}

		return this.ndef;
	}

}
