/**
 * @file main.cpp
 *
 * This is a sample NFC app.
 *
 * To enable NFC for an app, NFC permissions need to be set and an nfc.xml file
 * defining the type of tags to trigger upon.
 *
 * This example makes use of MoSync HTML5/JavaScript functionality for the
 * user interface
 *
 */

// Include Moblet for web applications.
#include <josync/WebAppMoblet.h>
#include <maapi.h>

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace josync; // Class WebAppMoblet

// A buffer size used as a basis for text buffers and data buffers.
#define BUF_SIZE 2048

// Icon constants -- these correspond to the png images found in
// the LocalFiles/images directory.
#define INFO_ICON "info"
#define ERROR_ICON "error"
#define READ_TAG_ICON "read_tag"
#define WRITE_TAG_ICON "write_tag"
#define CONTACT_ICON "contact"
#define URL_ICON "at"
#define UNKNOWN_ICON "unknown"

// The vCard MIME type
#define VCARD_MIME_TYPE "text/x-vcard"

// The amount of time to show temporary messages
#define TOAST_PERIOD 3000

// The mode the app is in; either read or write
enum Mode { _read, _write };

/**
 * The application class.
 */
class MyMoblet : public WebAppMoblet, TimerListener
{
private:
	byte payload[BUF_SIZE];
	char infoBuffer[3 * BUF_SIZE];
	char jsFnCall[3 * BUF_SIZE];
	MAHandle fCurrentTag;
	MAHandle sampleTag;
	Mode fMode;
	int exitOnBack;
public:

	MyMoblet() {
		initWebView();
	}

	void initWebView() {
		// Initialize the web view and show the main page.
		enableWebViewMessages();
		getWebView()->disableZoom();
		showPage("index.html");
	}

	/**
	 * Here we handle messages sent from JavaScript. Our example
	 * uses three messages:
	 * - StartNFC, at startup
	 * - WriteSampleURL, for writing an example NDEF URI
	 * - WriteSampleVCard, for writing an example NDEF vCard
	 */
	void handleWebViewMessage(WebViewMessage& message)
	{
		if (message.is("StartNFC")) {
			// First, let's start listening to NFC events...
			maNFCStart();
			// ...and we're in read more at this point.
			setMode(_read);
		} else if (message.is("WriteSampleURL")) {
			// If the user pressed the 'Write Web Address' button,
			// we enter write mode...
			setMode(_write);
			// ...and create a tag to write next time a tag is
			// held close to the device.
			sampleTag = createSampleURLTag();
		} else if (message.is("WriteSampleVCard")) {
			// Same as above, but a different type of data.
			setMode(_write);
			sampleTag = createSampleVCardTag();
		}
	}

	/**
	 * Handle key presses.
	 */
	void keyPressEvent(int keyCode, int nativeCode) {
		if (MAK_BACK == keyCode || MAK_0 == keyCode) {
			// If this flag is set, then quit.
			if (exitOnBack) {
				// Call close to exit the application.
				destroyCurrentTag();
				destroySampleTag();
				maNFCStop();
				close();
			} else { // Otherwise, we just puts the app in some initial state.
				setMode(_read);
			}
		}
	}

	/**
	 * The NFC events come through this function.
	 */
	void customEvent(const MAEvent& event) {
		// We always just keep track of one tag, and
		// this tag gets replaced by whatever new tag comes
		// close to the device:
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			MANFCEventData nfcEventData = event.nfc;
			MAHandle tag = maNFCReadTag(nfcEventData.handle);
			setCurrentTag(tag);
		}
		// Then, we just check whether we're in read or write
		// mode and the delegate to the proper
		// event handling code.
		switch (fMode) {
		case _write:
			customEventWrite(event);
			break;
		default:
			customEventRead(event);
		}
	}

	void setCurrentTag(MAHandle currentTag) {
		// Make sure to clean up first; otherwise we'd get a leak.
		destroyCurrentTag();
		fCurrentTag = currentTag;
	}

	void destroyCurrentTag() {
		if (fCurrentTag) {
			maNFCDestroyTag(fCurrentTag);
		}
	}

	void customEventRead(const MAEvent& event) {
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			if (fCurrentTag) {
				if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_NDEF)) {
					MAHandle ndef = maNFCGetTypedTag(fCurrentTag, MA_NFC_TAG_TYPE_NDEF);
					handleNDEF(ndef);
				} else if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_MIFARE_CL)) {
					MAHandle mfc = maNFCGetTypedTag(fCurrentTag, MA_NFC_TAG_TYPE_MIFARE_CL);
					handleMifare("Mifare Classic", mfc);
				} else if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_MIFARE_UL)) {
					MAHandle mfu = maNFCGetTypedTag(fCurrentTag, MA_NFC_TAG_TYPE_MIFARE_UL);
					int size = maNFCGetSize(mfu);
					handleMifare(size > 64 ? "Mifare Ultralight C" : "Mifare Ultralight", mfu);
				}
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
		}
	}

	void customEventWrite(const MAEvent& event) {
		if (EVENT_TYPE_NFC_TAG_RECEIVED == event.type) {
			MAHandle writeToTag = 0;
			if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_NDEF)) {
				writeToTag = maNFCGetTypedTag(fCurrentTag, MA_NFC_TAG_TYPE_NDEF);
			} else if (maNFCIsType(fCurrentTag, MA_NFC_TAG_TYPE_NDEF_FORMATTABLE)) {
				writeToTag = maNFCGetTypedTag(fCurrentTag, MA_NFC_TAG_TYPE_NDEF_FORMATTABLE);
			}

			if (writeToTag) {
				if (maNFCIsReadOnly(writeToTag) > 0) {
					setMode(_read);
					showStatus(ERROR_ICON, "Tag is read-only, cannot write.", TOAST_PERIOD);
				} else {
					writeSampleTag(writeToTag);
				}
			} else {
				setMode(_read);
				showStatus(ERROR_ICON, "Cannot write to this tag type.", TOAST_PERIOD);
			}
		} else if (EVENT_TYPE_NFC_TAG_DATA_WRITTEN == event.type) {
			MANFCEventData nfcEventData = event.nfc;
			MAHandle ndef = nfcEventData.handle;
			setMode(_read);
			if (nfcEventData.result >= 0) {
				// The dstId of NDEF write operations is the NDEF message handle
				int size = maNFCGetSize(nfcEventData.dstId);
				sprintf(infoBuffer, "Wrote tag. (%d bytes)", size);
				showStatus(INFO_ICON, infoBuffer, TOAST_PERIOD);
			} else {
				showStatus(ERROR_ICON, "Unable to write tag", TOAST_PERIOD);
			}
			maVibrate(100);
		}
	}

	void writeSampleTag(MAHandle ndef) {
		if (sampleTag) {
			maNFCConnectTag(ndef);
			maNFCWriteNDEFMessage(ndef, sampleTag);
			maNFCCloseTag(ndef);
		}
	}

	void setMode(Mode mode) {
		fMode = mode;
		switch (mode) {
		case _write:
			showStatus(WRITE_TAG_ICON, "<b>Ready to write</b><br/>Please hold a tag close to the device to write to it.<br/>Press BACK to cancel.", 0);
			maVibrate(100);
			break;
		default:
			showStatus(READ_TAG_ICON, "<b>Waiting for a tag</b><br/>Hold a tag close to the device to show its contents.", 0);
			exitOnBack = 1;
		}
	}

	/**
	 * Show a status message in the UI for a specified amount of time
	 * \param icon The icon to use (see constants above).
	 * \param msg The message to display
	 * \param period The amount of time (in ms) to wait before removing
	 * the message
	 */
	void showStatus(const char* icon, const char* msg, int period) {
		exitOnBack = 0;
		sprintf(jsFnCall, "updateInfo(\"%s\",\"%s\")", icon, msg);
		callJS(jsFnCall);
		if (period > 0) {
			addTimer(this, period, 1);
		} else {
			removeTimer(this);
		}
	}

	void destroySampleTag() {
		// Note that NDEF messages created
		// by the maNFCCreateNDEFMessage need
		// to be explicitly destroyed.
		if (sampleTag) {
			maNFCDestroyTag(sampleTag);
		}
	}

	/**
	 * Creates a sample URL NDEF message
	 */
	MAHandle createSampleURLTag() {
		destroySampleTag();
		MAHandle msg = maNFCCreateNDEFMessage(1);
		MAHandle rec = maNFCGetNDEFRecord(msg, 0);
		maNFCSetTnf(rec, MA_NFC_NDEF_TNF_WELL_KNOWN);
		byte type[] = {(byte) 0x55}; // URI
		maNFCSetType(rec, type, 1);
		char* mosync = "www.mosync.com";
		maNFCSetPayload(rec, mosync, strlen(mosync));
		return msg;
	}

	/**
	 * Creates a sample URL vCard message
	 */
	MAHandle createSampleVCardTag() {
		destroySampleTag();
		MAHandle msg = maNFCCreateNDEFMessage(1);
		MAHandle rec = maNFCGetNDEFRecord(msg, 0);
		maNFCSetTnf(rec, MA_NFC_NDEF_TNF_MIME_MEDIA);
		char* type = VCARD_MIME_TYPE;
		maNFCSetType(rec, type, strlen(VCARD_MIME_TYPE));
		char* vCard = "BEGIN:VCARD\n"
				"N:X;Mr\n"
				"END:VCARD";
		maNFCSetPayload(rec, vCard, strlen(vCard));
		return msg;
	}

	void handleNDEF(MAHandle tag) {
		MAHandle ndef = maNFCGetNDEFMessage(tag);
		if (ndef == 0) {
			maNFCConnectTag(tag);
			maNFCReadNDEFMessage(tag);
			maNFCCloseTag(tag);
		} else {
			int records = maNFCGetNDEFRecordCount(ndef);
			for (int i = 0; i < records; i++) {
				MAHandle record = maNFCGetNDEFRecord(ndef, i);
				int len = maNFCGetPayload(record, payload, BUF_SIZE);
				char* icon = getIcon(record);
				dumpPayloadAsText(icon, len);
			}
		}
	}

	void handleNDEFFormattable(MAHandle msg) {
		showStatus(INFO_ICON, "Empty tag", TOAST_PERIOD);
	}

	char* getIcon(MAHandle record) {
		byte type[256];
		char* icon = INFO_ICON;
		int tnf = maNFCGetTnf(record);
		size_t typeLen = maNFCGetType(record, type, 256);
		if (tnf == MA_NFC_NDEF_TNF_WELL_KNOWN && typeLen == 1 && type[0] == 0x55) {
			icon = URL_ICON;
		} else if (tnf == MA_NFC_NDEF_TNF_MIME_MEDIA && strncmp(VCARD_MIME_TYPE, (char*) type, typeLen)) {
			icon = CONTACT_ICON;
		}
		return icon;
	}

	void handleMifare(const char* type, MAHandle mfu) {
		int size = maNFCGetSize(mfu);
		sprintf(infoBuffer, "<b>%s</b><br/>Size: %d bytes", type, size);
		showStatus(UNKNOWN_ICON, infoBuffer, 0);
	}

	void dumpPayloadAsText(const char* icon, int len) {
		strcpy(infoBuffer, "<b>Contents</b><br/><![CDATA[");
		strncat(infoBuffer, (char*) payload, len);
		strcat(infoBuffer, "]]>");
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
