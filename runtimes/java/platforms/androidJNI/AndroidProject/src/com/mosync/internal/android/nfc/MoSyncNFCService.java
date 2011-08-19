package com.mosync.internal.android.nfc;

import android.app.IntentService;
import android.content.Intent;
import android.nfc.NdefMessage;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.os.Parcelable;

import com.mosync.java.android.MoSync;

public class MoSyncNFCService extends IntentService {

	public MoSyncNFCService(String name) {
		super(name);
	}

	@Override
	public void onHandleIntent(Intent intent) {
		String action = intent.getAction();
		MoSyncNFC nfcContext = MoSyncNFC.getDefault();
		if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)) {
			// If an NDEF tag is discovered, go ahead...
			// (the androidManifest.xml contains filters
			// for MIME/URI of the NDEF tag.
			Parcelable[] rawMsgs = intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
			NdefMessage[] msgs;
			if (rawMsgs != null) {
				msgs = new NdefMessage[rawMsgs.length];
				for (int i = 0; i < rawMsgs.length; i++) {
					msgs[i] = (NdefMessage) rawMsgs[i];
				}
				nfcContext.handleMessages(msgs);
				activateMoSyncApp();
			}
		} else if (NfcAdapter.ACTION_TECH_DISCOVERED.equals(action)) {
			// Or, we have some specific tag technology such as MIFARE
			// (also filtered in the androidManifest.xml)
			Tag tag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
			nfcContext.handleMessages(tag);
			activateMoSyncApp();
		}
	}

	private void activateMoSyncApp() {
		Intent launcherIntent = new Intent(this, MoSync.class);
		launcherIntent.addFlags(
			Intent.FLAG_ACTIVITY_NEW_TASK |
			Intent.FLAG_ACTIVITY_REORDER_TO_FRONT |
			Intent.FLAG_ACTIVITY_SINGLE_TOP);
		startActivity(launcherIntent);
	}

}
