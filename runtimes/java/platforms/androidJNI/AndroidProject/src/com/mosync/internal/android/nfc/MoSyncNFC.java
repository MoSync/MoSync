package com.mosync.internal.android.nfc;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_TAG_RECEIVED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_NFC_BATCH_OP;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_INVALID_TAG_TYPE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_NOT_AVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_NOT_ENABLED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_NDEF;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_CL;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_TAG_TYPE_MIFARE_UL;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

import android.app.Activity;
import android.content.Context;
import android.nfc.NdefMessage;
import android.nfc.NfcAdapter;
import android.nfc.NfcManager;
import android.nfc.Tag;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.nfc.MFCAuthenticateSectorWithKey.KeyType;
import com.mosync.internal.android.nfc.MoSyncNFC.NFCBatch;


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

	class NFCBatch extends ResourceBase implements RunnableWithResult<NFCEvent> {

		public NFCBatch(ResourcePool pool) {
			super(pool);
		}

		private final ArrayList<RunnableWithResult<NFCEvent>> operations = new ArrayList<RunnableWithResult<NFCEvent>>();

		public void addOperation(RunnableWithResult<NFCEvent> op) {
			this.operations.add(op);
		}

		@Override
		public NFCEvent run() {
			for (RunnableWithResult<NFCEvent> operation : operations) {
				NFCEvent result = operation.run();
				if (result.errorCodeOrLength < 0) {
					return new NFCEvent(EVENT_TYPE_NFC_BATCH_OP, getHandle(), -1, result.handle);
				}
			}
			return new NFCEvent(0, handle, 0, 0);
		}

	}

	class IOHandler implements Runnable {
		private Handler handler;
		private Looper looper;

		IOHandler() {

		}

		public void queue(final RunnableWithResult<NFCEvent> operation) {
			handler.post(new Runnable() {
				@Override
				public void run() {
					Log.d("@@@ÊMoSync NFC", "Executing operation: " + operation);
					NFCEvent result = operation.run();
					if (result != null) {
						postResult(result);
					}
				}
			});
		}

		public void start() {
			if (looper == null) {
				Thread thread = new Thread(this);
				thread.start();
			}
		}

		public void stop() {
			if (looper != null) {
				looper.quit();
				looper = null;
			}
		}

		@Override
		public void run() {
			Looper.prepare();
			looper = Looper.myLooper();
			handler = new Handler();
			Looper.loop();
		}
	}

	private static MoSyncNFC instance = new MoSyncNFC();

	private MoSyncThread mosyncThread;
	private final ResourcePool pool;

	private NFCTagHandler handler = null;
	private IOHandler ioHandler = null;
	private boolean queueIncomingTags;
	private final int handle;
	private HashMap<Integer, NFCBatch> currentBatches = null;

	private MoSyncNFC() {
		this.handler = new NFCTagHandler();
		this.ioHandler = new IOHandler();
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
		ioHandler.start();
	}

	private Activity getActivity() {
		return mosyncThread.getActivity();
	}

	public int maNFCStart() {
		NfcManager manager = (NfcManager) getActivity().getSystemService(Context.NFC_SERVICE);
		if (manager == null) {
			return MA_NFC_NOT_AVAILABLE;
		}

		NfcAdapter adapter = manager.getDefaultAdapter();
		if (adapter == null) {
			return MA_NFC_NOT_AVAILABLE;
		}
		if (!adapter.isEnabled()) {
			return MA_NFC_NOT_ENABLED;
		}

		init();
		sendEventIfPendingTags();

		return SUCCESS;
	}

	public void maNFCStop() {
		// Cleanup
		pool.destroyAll();
		queueIncomingTags = false;
		ioHandler.stop();
	}

	private void sendEventIfPendingTags() {
		if (mosyncThread != null && queueIncomingTags && handler.peek() != null) {
			postResult(new NFCEvent(EVENT_TYPE_NFC_TAG_RECEIVED, handle, 0, 0));
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

	public void maNFCDestroyTag(int tagHandle) {
		pool.destroy(tagHandle);
	}

	public int maNFCIsType(int tagHandle, int type) {
		// TODO: Supported technologies, how do we list them?
		if (type < MA_NFC_TAG_TYPE_NDEF || type > MA_NFC_TAG_TYPE_MIFARE_UL) {
			return MA_NFC_INVALID_TAG_TYPE;
		}
		return maNFCGetTypedTag(ResourcePool.NULL, tagHandle, type) == null ? 0 : 1;
	}

	public int maNFCGetTypedTag(int tagHandle, int type) {
		INFCTag typedTag = maNFCGetTypedTag(pool, tagHandle, type);
		return typedTag == null ? 0 : typedTag.getHandle();
	}

	public INFCTag maNFCGetTypedTag(ResourcePool pool, int tagHandle, int type) {
		IResource res = getResource(tagHandle);
		if (res instanceof INFCTag) {
			INFCTag typedTag = ((INFCTag) res).toTypedTag(pool, type);
			return typedTag;
		}
		return null;
	}

	public int maNFCBatchStart(int tagHandle) {
		if (currentBatches != null) {
			return MA_NFC_NOT_AVAILABLE;
		}
		NFCBatch currentBatch = new NFCBatch(pool);
		if (currentBatches == null) {
			currentBatches = new HashMap<Integer, MoSyncNFC.NFCBatch>();
		}
		currentBatches.put(tagHandle, currentBatch);
		return SUCCESS;
	}

	public void maNFCBatchCommit(int tagHandle) {
		NFCBatch currentBatch = getBatch(tagHandle);
		if (currentBatch != null) {
			ioHandler.queue(currentBatch);
			// Ok to start new batch.
			destroyBatch(tagHandle);
		}
	}

	public void maNFCBatchRollback(int tagHandle) {
		destroyBatch(tagHandle);
	}

	private void destroyBatch(int tagHandle) {
		NFCBatch currentBatch = getBatch(tagHandle);
		if (currentBatch != null) {
			currentBatch.destroy(pool);
			currentBatches.remove(tagHandle);
			if (currentBatches.isEmpty()) {
				currentBatches = null;
			}
		}
	}

	public ResourcePool getResourcePool() {
		return pool;
	}

	public void maNFCConnectTag(int tagHandle) {
		IResource tag = getResource(tagHandle);
		if (tag != null && tag instanceof INFCTag) {
			performIO(tagHandle, new TagConnect((INFCTag) tag));
		}
	}

	public void maNFCCloseTag(int tagHandle) {
		IResource tag = getResource(tagHandle);
		if (tag != null && tag instanceof INFCTag) {
			performIO(tagHandle, new TagClose((INFCTag) tag));
		}
	}


	public int maNFCGetNDEFMessage(int tagHandle) {
		IResource tag = getResource(tagHandle);
		if (tag instanceof NDEFMessage) {
			return tag.getHandle();
		}
		return NO_HANDLE;
	}

	public int maNFCGetNDEFRecord(int ndefHandle, int ix) {
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
		if (ndefMsg instanceof NDEFMessage) {
			return (NDEFMessage) ndefMsg;
		}
		return null;
	}

	public int maNFCGetNDEFRecordCount(int ndefHandle) {
		NDEFMessage msg = getNDEFMessage(ndefHandle);
		if (msg != null) {
			return msg.getRecordCount();
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public int maNFCGetId(int ndefRecordHandle, int dst, int len) {
		IResource ndefRecord = getResource(ndefRecordHandle);
		if (ndefRecord instanceof NDEFRecord) {
			NDEFRecord rec = (NDEFRecord) ndefRecord;
			return rec.maNFCGetId(getMemoryAt(dst, len));
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public int maNFCGetPayload(int ndefRecordHandle, int dst, int len) {
		IResource ndefRecord = getResource(ndefRecordHandle);
		if (ndefRecord instanceof NDEFRecord) {
			NDEFRecord rec = (NDEFRecord) ndefRecord;
			return rec.maNFCGetPayload(getMemoryAt(dst, len));
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public int maNFCGetTnf(int ndefRecordHandle) {
		IResource ndefRecord = getResource(ndefRecordHandle);
		if (ndefRecord instanceof NDEFRecord) {
			NDEFRecord rec = (NDEFRecord) ndefRecord;
			return rec.maNFCGetTnf();
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public int maNFCGetType(int ndefRecordHandle, int dst, int len) {
		IResource ndefRecord = getResource(ndefRecordHandle);
		if (ndefRecord instanceof NDEFRecord) {
			NDEFRecord rec = (NDEFRecord) ndefRecord;
			return rec.maNFCGetPayload(getMemoryAt(dst, len));
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public int maNFCAuthenticateSectorWithKeyA(int tagHandle, int sectorIndex, int keySrc, int keyLen) {
		return maNFCAuthenticateSectorWithKey(KeyType.A, tagHandle, sectorIndex, keySrc, keyLen);
	}

	public int maNFCAuthenticateSectorWithKeyB(int tagHandle, int sectorIndex, int keySrc, int keyLen) {
		return maNFCAuthenticateSectorWithKey(KeyType.B, tagHandle, sectorIndex, keySrc, keyLen);
	}

	private MifareClassicTag getMifareClassicTag(int tagHandle) {
		IResource res = getResource(tagHandle);
		if (res instanceof MifareClassicTag) {
			return (MifareClassicTag) res;
		}
		return null;
	}

	private int maNFCAuthenticateSectorWithKey(KeyType keyType, int tagHandle, int sectorIndex, int keySrc, int keyLen) {
		MifareClassicTag mifareTag = getMifareClassicTag(tagHandle);
		if (mifareTag != null) {
			final byte[] keyBuffer = new byte[keyLen];
			getMemoryAt(keySrc, keyLen).get(keyBuffer);
			performIO(tagHandle, new MFCAuthenticateSectorWithKey(tagHandle, keyType, keyBuffer, mifareTag.getTag(), sectorIndex));
			return SUCCESS;
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public int maNFCGetSectorCount(int tagHandle) {
		MifareClassicTag mifareTag = getMifareClassicTag(tagHandle);
		if (mifareTag != null) {
			return mifareTag.getTag().getSectorCount();
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public int maNFCGetBlockCountInSector(int tagHandle, int sectorIndex) {
		MifareClassicTag mifareTag = getMifareClassicTag(tagHandle);
		if (mifareTag != null) {
			return mifareTag.getTag().getBlockCountInSector(sectorIndex);
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public int maNFCSectorToBlock(int tagHandle, int sectorIndex) {
		MifareClassicTag mifareTag = getMifareClassicTag(tagHandle);
		if (mifareTag != null) {
			return mifareTag.getTag().sectorToBlock(sectorIndex);
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public int maNFCReadBlock(int tagHandle, int block, int dst, int resultSize) {
		MifareClassicTag mifareTag = getMifareClassicTag(tagHandle);
		if (mifareTag != null) {
			performIO(tagHandle, new MFCReadBlocks(tagHandle, mifareTag.getTag(), block, getMemoryAt(dst, resultSize)));
			return SUCCESS;
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	private MifareUltralightTag getMifareUltralightTag(int tagHandle) {
		IResource res = getResource(tagHandle);
		if (res instanceof MifareUltralightTag) {
			return (MifareUltralightTag) res;
		}
		return null;
	}

	public int maNFCReadPages(int tagHandle, int firstPage, int dst, int resultSize) {
		MifareUltralightTag mifareUTag = getMifareUltralightTag(tagHandle);
		if (mifareUTag != null) {
			performIO(tagHandle, new MFUReadPages(tagHandle, mifareUTag.getTag(), firstPage, getMemoryAt(dst, resultSize)));
			return SUCCESS;
		}
		return MA_NFC_INVALID_TAG_TYPE;
	}

	public static MoSyncNFC getDefault() {
		return instance;
	}

	private void handleTag(INFCTag tag) {
		handler.add(tag);
	}

	public void handleMessages(NdefMessage[] msgs) {
		for (NdefMessage msg : msgs) {
			NDEFMessage ndefMsg = new NDEFMessage(pool, msg);
			handleTag(ndefMsg);
		}
	}

	public void handleMessages(Tag tag) {
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

	private void postResult(NFCEvent event) {
		int[] nativeEvent = new int[] {
				event.eventType,
				event.handle,
				event.errorCodeOrLength,
				event.dst
			};
		mosyncThread.postEvent(nativeEvent);
		Log.d("@@@ MoSync NFC event", event.toString());
	}

	private void performIO(int tagHandle, RunnableWithResult<NFCEvent> runnable) {
		NFCBatch currentBatch = getBatch(tagHandle);
		if (currentBatch != null) {
			currentBatch.addOperation(runnable);
		} else {
			ioHandler.queue(runnable);
		}
	}

	private NFCBatch getBatch(int tagHandle) {
		return currentBatches == null ? null : currentBatches.get(tagHandle);
	}
}
