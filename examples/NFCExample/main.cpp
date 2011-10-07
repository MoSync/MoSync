/**
 * @file main.cpp
 *
 * Sample application that illustrates how to call into C++
 * from JavaScript.
 */

// Include Moblet for web applications.
#include <josync/WebAppMoblet.h>
#include <maapi.h>

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace josync; // Class WebAppMoblet

#define BUF_SIZE 2048
#define INFO_ICON "info"
#define ERROR_ICON "error"
#define READ_TAG_ICON "read_tag"
#define WRITE_TAG_ICON "write_tag"
#define CONTACT_ICON "contact"
#define URL_ICON "at"
#define TOAST_PERIOD 5000

enum Mode { _read, _write };

/**
 * The application class.
 */
class MyMoblet : public WebAppMoblet, TimerListener
{
private:
	byte payload[BUF_SIZE];
	byte type[BUF_SIZE];
	char hexEncoded[2 * BUF_SIZE];
	char infoBuffer[3 * BUF_SIZE];
	char jsFnCall[3 * BUF_SIZE];
	const char* HEX_CHARS;
	MAHandle sampleTag;
	Mode fMode;
public:

	MyMoblet()
	{
		initWebView();
		HEX_CHARS = "0123456789ABCDEF";
	}

	void initWebView() {
		enableWebViewMessages();
		getWebView()->disableZoom();
		showPage("index.html");
	}

	int initNFC() {
		return maNFCStart();
	}

	/**
	 * Here we handle messages sent from JavaScript.
	 */
	void handleWebViewMessage(WebViewMessage& message)
	{
		if (message.is("StartNFC")) {
			initNFC();
			setMode(_read);
		}
		if (message.is("WriteSampleTag")) {
			setMode(_write);
			maVibrate(100);
		}
	}

	/**
	 * Quit the program if back key or 0 is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode) {
		if (MAK_BACK == keyCode || MAK_0 == keyCode) {
			if (fMode == _write) {
				setMode(_read);
			} else {
				// Call close to exit the application.
				close();
			}
		}
	}

	void customEvent(const MAEvent& event) {
		switch (fMode) {
		case _write:
			customEventWrite(event);
			break;
		default:
			customEventRead(event);
		}
	}

	void customEventRead(const MAEvent& event) {
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			MANFCEventData nfcEventData = event.nfc;
			MAHandle tag = maNFCReadTag(nfcEventData.handle);
			if (tag) {
				if (maNFCIsType(tag, MA_NFC_TAG_TYPE_NDEF)) {
					MAHandle ndef = maNFCGetTypedTag(tag, MA_NFC_TAG_TYPE_NDEF);
					handleNDEF(ndef);
				} else if (maNFCIsType(tag, MA_NFC_TAG_TYPE_MIFARE_CL)) {
					MAHandle mfc = maNFCGetTypedTag(tag,
							MA_NFC_TAG_TYPE_MIFARE_CL);
					handleMifareClassic(mfc);
				} else if (maNFCIsType(tag, MA_NFC_TAG_TYPE_MIFARE_UL)) {
					MAHandle mfu = maNFCGetTypedTag(tag,
							MA_NFC_TAG_TYPE_MIFARE_UL);
					handleMifareUltralight(mfu);
				}
				maNFCDestroyTag(tag);
			}
		} else if (EVENT_TYPE_NFC_TAG_DATA_READ == event.type) {
			MANFCEventData& data = (MANFCEventData&) event.nfc;
			int tag = data.handle;
			if (data.result < 0) {
				sprintf(infoBuffer, "Failed to read tag (try again.) Error code: %d", data.result);
				showStatus(ERROR_ICON, infoBuffer, TOAST_PERIOD);
			} else if (maNFCIsType(tag, MA_NFC_TAG_TYPE_NDEF)) {
				handleNDEF(tag);
			}
			maNFCDestroyTag(tag);
		}
	}

	void customEventWrite(const MAEvent& event) {
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			MANFCEventData nfcEventData = event.nfc;
			MAHandle tag = maNFCReadTag(nfcEventData.handle);
			if (tag && maNFCIsType(tag, MA_NFC_TAG_TYPE_NDEF)) {
				MAHandle ndef = maNFCGetTypedTag(tag, MA_NFC_TAG_TYPE_NDEF);
				maNFCDestroyTag(tag);
				if (maNFCIsReadOnly(ndef) > 0) {
					showStatus(ERROR_ICON, "Tag is read-only, cannot write.", TOAST_PERIOD);
					setMode(_read);
				} else {
					writeSampleTag(ndef);
				}
			}
		} else if (EVENT_TYPE_NFC_TAG_DATA_WRITTEN == event.type) {
			MANFCEventData nfcEventData = event.nfc;
			MAHandle ndef = nfcEventData.handle;
			if (maNFCIsType(ndef, MA_NFC_TAG_TYPE_NDEF)) {
				maNFCDestroyTag(ndef);
				if (sampleTag != 0) {
					maNFCDestroyTag(sampleTag);
					setMode(_read);
				}
				showStatus(INFO_ICON, "Wrote tag.", TOAST_PERIOD);
				maVibrate(100);
			}
		}
	}

	void writeSampleTag(MAHandle ndef) {
		sampleTag = createSampleTag();
		maNFCConnectTag(ndef);
		maNFCWriteNDEFMessage(ndef, sampleTag);
		maNFCCloseTag(ndef);
	}

	void setMode(Mode mode) {
		fMode = mode;
		switch (mode) {
		case _write:
			showStatus(WRITE_TAG_ICON, "<b>Ready to write</b><br/>Please hold a tag close to the device to write to it.<br/>Press BACK to cancel.", 0);
			break;
		default:
			showStatus(READ_TAG_ICON, "<b>Waiting for a tag</b><br/>Hold a tag close to the device to show its contents.", 0);
		}
	}

	void showStatus(const char* icon, const char* info, int period) {
		sprintf(jsFnCall, "updateInfo(\"%s\",\"%s\")", icon, info);
		callJS(jsFnCall);
		if (period > 0) {
			addTimer(this, period, 1);
		} else {
			removeTimer(this);
		}
	}

	MAHandle createSampleTag() {
		MAHandle msg = maNFCCreateNDEFMessage(1);
		MAHandle rec = maNFCGetNDEFRecord(msg, 0);
		maNFCSetTnf(rec, 0x01); // TNF well known -- fix constants!
		byte type[] = {(byte) 0x55}; // URI
		maNFCSetType(rec, type, 1);
		char* mosync = "www.mosync.com";
		maNFCSetPayload(rec, mosync, strlen(mosync));
		return msg;
	}

	void handleNDEF(MAHandle msg) {
		MAHandle ndef = maNFCGetNDEFMessage(msg);
		if (ndef == 0) {
			maNFCConnectTag(msg);
			maNFCReadNDEFMessage(msg);
			maNFCCloseTag(msg);
		} else {
			int records = maNFCGetNDEFRecordCount(ndef);
			for (int i = 0; i < records; i++) {
				MAHandle record = maNFCGetNDEFRecord(ndef, i);
				int len = maNFCGetPayload(record, payload, BUF_SIZE);
				int tnf = maNFCGetTnf(record);
				maNFCGetType(record, type, BUF_SIZE);
				char* icon = INFO_ICON;
				if (tnf == 0x1 && type[0] == 0x55) {
					icon = URL_ICON;
				}
				dumpPayloadAsText(icon, len);
			}
		}
	}

	void handleMifareClassic(MAHandle mfc) {
		int size = 0;
		int sectors = maNFCGetSectorCount(mfc);
		for (int i = 0; i < sectors; i++) {
			int blocks = maNFCGetBlockCountInSector(mfc, i);
			// Every block is 4 bytes
			size = size + 4 * blocks;
		}
		sprintf(infoBuffer, "<b>Mifare Classic tag</b><br/>Size: %d bytes", size);
		showStatus(INFO_ICON, infoBuffer, 0);
	}

	void handleMifareUltralight(MAHandle mfu) {
		maNFCConnectTag(mfu);
		maNFCReadPages(mfu, 0, payload, BUF_SIZE);
		maNFCCloseTag(mfu);
	}

	void toHex(byte* data, int len, char* output) {
		for (int i = 0; i < len; i++) {
			output[2 * i] = HEX_CHARS[(data[i] >> 4) & 0xf];
			output[2 * i + 1] = HEX_CHARS[data[i] & 0xf];
		}
		output[2 * len] = '\0';
	}

	void dumpPayloadAsText(const char* icon, int len) {
		sprintf(infoBuffer, "<b>Contents</b><br/>%s", payload);
		showStatus(icon, infoBuffer, 0);
	}

	void closeEvent() {
		// Ok, we're quitting -- stop listening to NFC events.
		maNFCStop();
	}

	void runTimerEvent() {
		// Just show the default message
		setMode(fMode);
	}
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
