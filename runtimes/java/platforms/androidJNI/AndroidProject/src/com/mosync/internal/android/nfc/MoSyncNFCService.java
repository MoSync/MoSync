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

import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.util.Log;

import com.mosync.java.android.MoSync;

public class MoSyncNFCService extends IntentService {

	public MoSyncNFCService(String name) {
		super(name);
	}

	@Override
	public void onHandleIntent(Intent intent) {
		handleNFCIntent(this, intent);
	}

	public static MoSyncNFC getDefault() {
		try {
			return MoSyncNFC.getDefault();
		} catch (Throwable t) { // In case of a verify error
			Log.e("@@@ MoSync", "NFC not available");
			return null;
		}
	}

	/**
	 * Handles an NFC intent
	 *
	 * @param context
	 * @param intent
	 * @return {@code true} If the intent was an NFC intent and handled.
	 */
	public static boolean handleNFCIntent(Context context, Intent intent) {
		String action = intent.getAction();
		MoSyncNFC nfcContext = getDefault();
		if (nfcContext == null) {
			return false;
		}

		// ACTION_NDEF_DISCOVERED NOT YET SUPPORTED (because it complicates the
		// APIs a bit)
		// Uncomment later if needed.
		/*
		 * if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)) { // If an NDEF
		 * tag is discovered, go ahead... // (the androidManifest.xml contains
		 * filters // for MIME/URI of the NDEF tag.) Parcelable[] rawMsgs =
		 * intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
		 * NdefMessage[] msgs; if (rawMsgs != null) { msgs = new
		 * NdefMessage[rawMsgs.length]; for (int i = 0; i < rawMsgs.length; i++)
		 * { msgs[i] = (NdefMessage) rawMsgs[i]; }
		 * nfcContext.handleMessages(msgs); activateMoSyncApp(context); return
		 * true; } } else
		 */if (NfcAdapter.ACTION_TECH_DISCOVERED.equals(action)
				|| NfcAdapter.ACTION_TAG_DISCOVERED.equals(action)) {
			// Or, we have some specific tag technology such as MIFARE
			// (also filtered in the androidManifest.xml)
			Tag tag = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);
			nfcContext.handleMessages(tag);
			activateMoSyncApp(context);
			return true;
		}

		return false;
	}

	private static void activateMoSyncApp(Context context) {
		Intent launcherIntent = new Intent(context, MoSync.class);
		launcherIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
				| Intent.FLAG_ACTIVITY_REORDER_TO_FRONT
				| Intent.FLAG_ACTIVITY_SINGLE_TOP);
		context.startActivity(launcherIntent);
	}

}
