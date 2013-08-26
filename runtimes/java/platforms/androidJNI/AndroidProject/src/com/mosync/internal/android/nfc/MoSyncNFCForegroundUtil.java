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

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.IntentFilter;
import android.nfc.NfcAdapter;
import android.nfc.tech.IsoDep;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.MifareUltralight;
import android.nfc.tech.Ndef;
import android.nfc.tech.NdefFormatable;
import android.nfc.tech.NfcA;
import android.nfc.tech.NfcB;
import android.nfc.tech.NfcF;
import android.nfc.tech.NfcV;

public class MoSyncNFCForegroundUtil {
	private final NfcAdapter nfc;

	private final Activity activity;

	private final PendingIntent intent;

	private final IntentFilter[] intentFilters;

	private final String[][] techLists;

	private MoSyncNFCForegroundUtil(Activity activity, NfcAdapter nfc) {
	    this.activity = activity;
	    this.nfc = nfc;

	    intent = PendingIntent.getActivity(activity, 0, new Intent(activity, activity.getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);

	    // TODO: NDEF!?
	    IntentFilter ndef = new IntentFilter(NfcAdapter.ACTION_TECH_DISCOVERED);

//	    try {
//	        ndef.addDataType("*/*");
//	    } catch (MalformedMimeTypeException e) {
//	        throw new RuntimeException("Unable to speciy */* Mime Type", e);
//	    }

	    intentFilters = new IntentFilter[] { ndef };

	    // TODO: READ FROM NFC.XML OR SMTHG
	    techLists = new String[][] {
		    	new String[] { NfcA.class.getName() },
		    	new String[] { NfcB.class.getName() },
		    	new String[] { NfcF.class.getName() },
		    	new String[] { NfcV.class.getName() },
		    	new String[] { MifareClassic.class.getName() },
		    	new String[] { MifareUltralight.class.getName() },
		    	new String[] { IsoDep.class.getName() },
		    	new String[] { Ndef.class.getName() },
		    	new String[] { NdefFormatable.class.getName() }
	    };
	}

	public void enableForeground() {
		if (nfc != null) {
			nfc.enableForegroundDispatch(activity, intent, intentFilters, techLists);
		}
	}

	public void disableForeground() {
		nfc.disableForegroundDispatch(activity);
	}

	public NfcAdapter getNfc() {
	    return nfc;
	}

	/**
	 * Returns an instanceof a this class IF NFC privileges have been set.
	 * @param moSync
	 * @return
	 */
	public static MoSyncNFCForegroundUtil create(Activity activity) {
		try {
		if (MoSyncNFC.nfcPermissionsSet(activity)) {
		    NfcAdapter nfc = NfcAdapter.getDefaultAdapter(activity.getApplicationContext());
			if (nfc != null) {
				return new MoSyncNFCForegroundUtil(activity, nfc);
			}
		}
		} catch (Throwable t) {
			// We could end having a VerifyError or something.
		}
		return null;
	}
}
