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

enum Mode { _read, _write };

/**
 * The application class.
 */
class MyMoblet : public WebAppMoblet
{
private:
	byte payload[BUF_SIZE];
	char hexEncoded[2 * BUF_SIZE];
	char infoBuffer[3 * BUF_SIZE];
	char jsFnCall[3 * BUF_SIZE];
	const char* HEX_CHARS;
	MAHandle sampleTag;
	Mode mode;
public:

	MyMoblet()
	{
		initWebView();
		mode = _read;
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
		}
		if (message.is("Vibrate"))
		{
			// Just some tactile user feedback
			maVibrate(100);
		} else if (message.is("WriteSampleTag")) {
			mode = _write;
		}
	}

	/**
	 * Quit the program if back key or 0 is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode) {
		if (MAK_BACK == keyCode || MAK_0 == keyCode) {
			if (mode == _write) {
				mode = _read;
				info(INFO_ICON, "Cancelled write.");
			} else {
				// Call close to exit the application.
				close();
			}
		}
	}

	void customEvent(const MAEvent& event) {
		if (EVENT_TYPE_NFC_TAG_DATA_WRITTEN == event.type) {
			if (maNFCIsType(event.nfc.handle, MA_NFC_TAG_TYPE_NDEF)) {
				maNFCDestroyTag(event.nfc.handle);
				if (sampleTag != 0) {
					maNFCDestroyTag(sampleTag);
					mode = _read;
				}
			}
		}
		switch (mode) {
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
				info(ERROR_ICON, infoBuffer);
			} else {
				if (maNFCIsType(tag, MA_NFC_TAG_TYPE_NDEF)) {
					handleNDEF(tag);
				} else {
					if (data.result > 0) {
						dumpPayload(data.result);
						dumpPayloadAsText(data.result);
					} else {

					}
				}
			}
			maNFCDestroyTag(tag);
		}
	}

	void info(const char* icon, const char* info) {
		sprintf(jsFnCall, "updateInfo(\"%s\",\"%s\")", icon, info);
		callJS(jsFnCall);
	}

	void customEventWrite(const MAEvent& event) {
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			MANFCEventData nfcEventData = event.nfc;
			MAHandle tag = maNFCReadTag(nfcEventData.handle);
			if (tag && maNFCIsType(tag, MA_NFC_TAG_TYPE_NDEF)) {
				sampleTag = createSampleTag();
				MAHandle ndef = maNFCGetTypedTag(tag, MA_NFC_TAG_TYPE_NDEF);
				maNFCDestroyTag(tag);
				maNFCConnectTag(ndef);
				maNFCWriteNDEFMessage(ndef, sampleTag);
				maNFCCloseTag(ndef);
			}
		}
	}

	MAHandle createSampleTag() {
		MAHandle msg = maNFCCreateNDEFMessage(1);
		MAHandle rec = maNFCGetNDEFRecord(msg, 0);
		maNFCSetTnf(rec, 0x01); // TNF well known -- fix constants!
		byte id[] = {(byte) 0x55};
		maNFCSetType(rec, id, 1);
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
			sprintf(infoBuffer, "Record count: %i", records);
			info(INFO_ICON, infoBuffer);
			for (int i = 0; i < records; i++) {
				MAHandle record = maNFCGetNDEFRecord(ndef, i);
				int len = maNFCGetPayload(record, payload, BUF_SIZE);
				//dumpPayload(len);
				dumpPayloadAsText(len);
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
		sprintf(infoBuffer, "Size: %d bytes", size);
		info(INFO_ICON, infoBuffer);
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

	void dumpPayload(int len) {
		toHex(payload, len, hexEncoded);
		sprintf(infoBuffer, "Read tag, payload: %s", hexEncoded);
		info(INFO_ICON, infoBuffer);
	}

	void dumpPayloadAsText(int len) {
		sprintf(infoBuffer, "Read tag, payload as text: %s", payload);
		info(INFO_ICON, infoBuffer);
	}

	void closeEvent() {
		// Ok, we're quitting -- stop listening to NFC events.
		maNFCStop();
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
