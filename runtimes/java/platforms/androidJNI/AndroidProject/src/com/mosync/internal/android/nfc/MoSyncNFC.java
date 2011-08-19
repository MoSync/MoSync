package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_RECEIVED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_MA_NFC_INVALID_TAG_TYPE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_NOT_AVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_NOT_ENABLED;

import java.nio.ByteBuffer;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

import android.app.Activity;
import android.content.Context;
import android.nfc.NdefMessage;
import android.nfc.NfcAdapter;
import android.nfc.NfcManager;
import android.nfc.Tag;

import com.mosync.internal.android.MoSyncThread;


public class MoSyncNFC {

	public final static int NO_HANDLE = 0;
	public final static int SUCCESS = 0;

	class NFCTagHandler {

		// No more events than this, ever.
		final static int MAX_BUFFER_SIZE = 48;

		private final ConcurrentLinkedQueue<INFCTag> tags = new ConcurrentLinkedQueue<INFCTag>();

		private final AtomicInteger size = new AtomicInteger();

		// Only one thread may access this one
		public void add(INFCTag newTag) {
			tags.offer(newTag);
			size.incrementAndGet();
			if (size.get() > MAX_BUFFER_SIZE) {
				// Throw away
				INFCTag tag = poll();
				tag.destroy(pool);
			}
			sendEventIfPendingTags();
		}

		public INFCTag poll() {
			INFCTag result = tags.poll();
			if (result != null) {
				size.decrementAndGet();
			}
			return result;
		}

		public INFCTag peek() {
			return tags.peek();
		}
	}

	private static MoSyncNFC instance = new MoSyncNFC();

	private MoSyncThread mosyncThread;
	private final ResourcePool pool;

	private NFCTagHandler handler = new NFCTagHandler();
	private boolean queueIncomingTags;
	private final int handle;

	private MoSyncNFC() {
		this.handler = new NFCTagHandler();
		this.pool = new ResourcePool();
		this.handle = 1; // Singleton
	}

	/**
	 * Sets the {@link MoSyncThread} of the NFC component.
	 * Since the NFC component oftentimes get called before
	 * there even is a {@link MoSyncThread}, we cannot
	 * pass it in using the constructor.
	 */
	public void setMoSyncThread(MoSyncThread mosyncThread) {
		this.mosyncThread = mosyncThread;
		// A new thread? Then we'll always start listening to events.
		init();
	}

	private void init() {
		queueIncomingTags = true;
	}

	private Activity getActivity() {
		return mosyncThread.getActivity();
	}

	public int maStartNFC() {
		NfcManager manager = (NfcManager) getActivity().getSystemService(Context.NFC_SERVICE);
		if (manager == null) {
			return MA_NFC_NOT_AVAILABLE;
		}

		NfcAdapter adapter = manager.getDefaultAdapter();
		if (!adapter.isEnabled()) {
			return MA_NFC_NOT_ENABLED;
		}

		init();
		sendEventIfPendingTags();

		return SUCCESS;
	}

	public void maStopNFC() {
		// Cleanup
		pool.destroyAll();
		queueIncomingTags = false;
	}

	private void sendEventIfPendingTags() {
		if (mosyncThread != null && queueIncomingTags && handler.peek() != null) {
			int[] event = new int[3];
			event[0] = EVENT_TYPE_NFC_TAG_RECEIVED;
			event[1] = handle;
			mosyncThread.postEvent(event);
		}
	}

	public int maNFCReadTag(int nfcContext) {
		// We currently only have one nfc context...
		INFCTag tag = handler.poll();
		if (tag != null) {
			return tag.getHandle();
		}
		return NO_HANDLE;
	}

	void maNFCDestroyTag(int tagHandle) {
		maNFCCloseTag(tagHandle);
		pool.destroy(tagHandle);
	}

	public void maBatchStart(int nfcContext) {

	}

	public void maBatchCommit(int nfcContext) {

	}

	public void maBatchRollback(int nfcContext) {

	}

	public ResourcePool getResourcePool() {
		return pool;
	}

	int maNFCConnectTag(int tagHandle) {
		IResource tag = getResource(tagHandle);
		if (tag != null && tag instanceof INFCTag) {
			((INFCTag) tag).connect();
		}
		return SUCCESS;
	}

	int maNFCCloseTag(int tagHandle) {
		IResource tag = getResource(tagHandle);
		if (tag != null && tag instanceof INFCTag) {
			((INFCTag) tag).close();
		}
		return SUCCESS;
	}


	int maNFCGetNDEFMessage(int tagHandle) {
		IResource tag = getResource(tagHandle);
		if (tag instanceof NDEFMessage) {
			return tag.getHandle();
		}
		return NO_HANDLE;
	}

	int maNFCGetNDEFRecord(int ndefHandle, int ix) {
		NDEFMessage msg = getNDEFMessage(ndefHandle);
		if (msg != null) {
			NDEFRecord record = msg.getRecord(ix);
			if (record != null) {
				return record.getHandle();
			}
		}
		return NO_HANDLE;
	}

	private NDEFMessage getNDEFMessage(int ndefHandle) {
		IResource ndefMsg = getResource(ndefHandle);
		if (ndefMsg instanceof INFCTag) {
			INFCTag tag = (INFCTag) ndefMsg;
			NDEFMessage msg = tag.getNDEFMessage(pool);
			return msg;
		}
		return null;
	}

	int maNFCGetNDEFRecordCount(int ndefHandle) {
		NDEFMessage msg = getNDEFMessage(ndefHandle);
		if (msg != null) {
			return msg.getRecordCount();
		}
		return MA_NFC_MA_NFC_INVALID_TAG_TYPE;
	}

	int maNFCGetId(int ndefRecordHandle, int dst, int len) {
		IResource ndefRecord = getResource(ndefRecordHandle);
		if (ndefRecord instanceof NDEFRecord) {
			NDEFRecord rec = (NDEFRecord) ndefRecord;
			return rec.maNFCGetId(getMemoryAt(dst, len));
		}
		return MA_NFC_MA_NFC_INVALID_TAG_TYPE;
	}

	int maNFCGetPayload(int ndefRecordHandle, int dst, int len) {
		IResource ndefRecord = getResource(ndefRecordHandle);
		if (ndefRecord instanceof NDEFRecord) {
			NDEFRecord rec = (NDEFRecord) ndefRecord;
			return rec.maNFCGetPayload(getMemoryAt(dst, len));
		}
		return MA_NFC_MA_NFC_INVALID_TAG_TYPE;
	}

	int maNFCGetTnf(int ndefRecordHandle) {
		IResource ndefRecord = getResource(ndefRecordHandle);
		if (ndefRecord instanceof NDEFRecord) {
			NDEFRecord rec = (NDEFRecord) ndefRecord;
			return rec.maNFCGetTnf();
		}
		return MA_NFC_MA_NFC_INVALID_TAG_TYPE;
	}

	int maNFCGetType(int ndefRecordHandle, int dst, int len) {
		IResource ndefRecord = getResource(ndefRecordHandle);
		if (ndefRecord instanceof NDEFRecord) {
			NDEFRecord rec = (NDEFRecord) ndefRecord;
			return rec.maNFCGetPayload(getMemoryAt(dst, len));
		}
		return MA_NFC_MA_NFC_INVALID_TAG_TYPE;
	}

	int maMFCAuthenticateSectorWithKeyA(int tagHandle, int sector, byte[] key, int keyLen) {
		return NO_HANDLE;
	}

	int maMFCAuthenticateSectorWithKeyB(int tagHandle, int sector, byte[] key, int keyLen) {
		return NO_HANDLE;
	}

	int maMFCGetSectorCount(int tagHandle) {
		return NO_HANDLE;
	}

	int maMFCGetBlockCountInSector(int tagHandle, int sector) {
		return NO_HANDLE;
	}

	int maMFCSectorToBlock(int tagHandle, int sector) {
		return NO_HANDLE;
	}

	int maMFCReadBlock(int tagHandle, int block, byte[] result, int resultSize) {
		return NO_HANDLE;
	}

	int maMFUReadFourPages(int tagHandle, int firstPage, byte[] result, int resultSize) {
		return NO_HANDLE;
	}

	public static MoSyncNFC getDefault() {
		return instance;
	}

	private void handleTag(INFCTag tag) {
		handler.add(tag);
	}

	void handleMessages(NdefMessage[] msgs) {
		for (NdefMessage msg : msgs) {
			NDEFMessage ndefMsg = new NDEFMessage(pool, msg);
			handleTag(ndefMsg);
		}
	}

	void handleMessages(Tag tag) {
		handleTag(new GenericTag(pool, tag));
	}

	private IResource getResource(int tagHandle) {
		return pool.getResource(tagHandle);
	}

	private ByteBuffer getMemoryAt(int offset, int bound) {
		mosyncThread.mMemDataSection.position(offset);
		ByteBuffer slice = mosyncThread.mMemDataSection.slice();
		slice.limit(bound);
		return slice;
	}
}
