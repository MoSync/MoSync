package com.mosync.internal.android.nfc;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.IntentFilter.MalformedMimeTypeException;
import android.nfc.NfcAdapter;
import android.nfc.tech.NfcA;
import android.nfc.tech.NfcB;
import android.nfc.tech.NfcV;

public class MoSyncNFCForegroundUtil {
	private final NfcAdapter nfc;

	private final Activity activity;

	private final PendingIntent intent;

	private final IntentFilter[] intentFilters;

	private final String[][] techLists;

	public MoSyncNFCForegroundUtil(Activity activity) {
	    this.activity = activity;
	    nfc = NfcAdapter.getDefaultAdapter(activity.getApplicationContext());

	    intent = PendingIntent.getActivity(activity, 0, new Intent(activity, activity.getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);

	    IntentFilter ndef = new IntentFilter(NfcAdapter.ACTION_NDEF_DISCOVERED);

	    try {
	        ndef.addDataType("*/*");
	    } catch (MalformedMimeTypeException e) {
	        throw new RuntimeException("Unable to speciy */* Mime Type", e);
	    }
	    intentFilters = new IntentFilter[] { ndef };

	    //techListsArray = new String[][] { new String[] { NfcA.class.getName() } };
	    techLists = new String[][] { new String[] { NfcA.class.getName(), NfcB.class.getName() }, new String[] {NfcV.class.getName()} };
	}

	public void enableForeground() {
	    nfc.enableForegroundDispatch(activity, intent, intentFilters, techLists);
	}

	public void disableForeground() {
		nfc.disableForegroundDispatch(activity);
	}

	public NfcAdapter getNfc() {
	    return nfc;
	}
}
