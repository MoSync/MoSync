using System;
namespace MoSync {
namespace Struct {
	public class MAPoint2d {
		public const int x = 0;
		public const int y = 4;
	}
	public class MARect {
		public const int left = 0;
		public const int top = 4;
		public const int width = 8;
		public const int height = 12;
	}
	public class MACopyData {
		public const int dst = 0;
		public const int dstOffset = 4;
		public const int src = 8;
		public const int srcOffset = 12;
		public const int size = 16;
	}
	public class MAConnAddrInet4 {
		public const int addr = 0;
		public const int port = 4;
	}
	public class MABtAddr {
		public const int a = 0;
	}
	public class MAConnAddrBt {
		public const int port = 0;
		public class addr {
			public const int a = 4;
		}
	}
	public class MAConnAddr {
		public const int family = 0;
		public class inet4 {
			public const int addr = 4;
			public const int port = 8;
		}
		public class bt {
			public const int port = 4;
			public class addr {
				public const int a = 8;
			}
		}
	}
	public class MAConnEventData {
		public const int handle = 0;
		public const int opType = 4;
		public const int result = 8;
	}
	public class MASensor {
		public const int type = 0;
		public const int values = 4;
	}
	public class MANFCEventData {
		public const int handle = 0;
		public const int result = 4;
		public const int dstId = 8;
	}
	public class MAAdsEventData {
		public const int bannerHandle = 0;
		public const int bannerEventType = 4;
		public const int bannerErrorCode = 8;
	}
	public class MACaptureEventData {
		public const int type = 0;
		public const int handle = 4;
	}
	public class MAPurchaseEventData {
		public const int type = 0;
		public const int state = 4;
		public const int productHandle = 8;
		public const int errorCode = 12;
	}
	public class MAEvent {
		public const int type = 0;
		public const int key = 4;
		public const int nativeKey = 8;
		public const int character = 4;
		public class point {
			public const int x = 4;
			public const int y = 8;
		}
		public const int touchId = 12;
		public const int state = 4;
		public class conn {
			public const int handle = 4;
			public const int opType = 8;
			public const int result = 12;
		}
		public const int textboxResult = 4;
		public const int textboxLength = 8;
		public const int imagePickerState = 4;
		public const int imagePickerItem = 8;
		public const int optionsBoxButtonIndex = 4;
		public const int alertButtonIndex = 4;
		public const int status = 4;
		public class sensor {
			public const int type = 4;
			public const int values = 8;
		}
		public class nfc {
			public const int handle = 4;
			public const int result = 8;
			public const int dstId = 12;
		}
		public class adsData {
			public const int bannerHandle = 4;
			public const int bannerEventType = 8;
			public const int bannerErrorCode = 12;
		}
		public const int localNotificationHandle = 4;
		public const int pushNotificationHandle = 4;
		public class captureData {
			public const int type = 4;
			public const int handle = 8;
		}
		public const int data = 4;
		public const int audioInstance = 4;
		public class purchaseData {
			public const int type = 4;
			public const int state = 8;
			public const int productHandle = 12;
			public const int errorCode = 16;
		}
	}
	public class MAPanicReport {
		public const int runtimeId = 0;
		public const int reportType = 4;
		public const int time = 8;
		public const int ip = 12;
		public const int code = 16;
		public const int string_ = 20;
	}
	public class MABtDevice {
		public const int name = 0;
		public const int nameBufSize = 4;
		public const int actualNameLength = 8;
		public class address {
			public const int a = 12;
		}
	}
	public class MAUUID {
		public const int i = 0;
	}
	public class MABtServiceSize {
		public const int nameBufSize = 0;
		public const int nUuids = 4;
	}
	public class MABtService {
		public const int port = 0;
		public const int name = 4;
		public const int nameBufSize = 8;
		public const int uuids = 12;
	}
	public class MAWlanAddr {
		public const int a = 0;
	}
	public class MAWlanAp {
		public class address {
			public const int a = 0;
		}
		public const int name = 6;
		public const int nameBufSize = 10;
		public const int signalStrength = 14;
	}
	public class MALocation {
		public const int state = 0;
		public const int lat = 4;
		public const int lon = 12;
		public const int horzAcc = 20;
		public const int vertAcc = 28;
		public const int alt = 36;
	}
	public class MACellInfo {
		public const int mcc = 0;
		public const int mnc = 4;
		public const int lac = 12;
		public const int cellId = 16;
	}
	public class MAStreamEventData {
		public const int event_ = 0;
		public const int stream = 4;
		public const int result = 8;
	}
	public class MAFrameBufferInfo {
		public const int sizeInBytes = 0;
		public const int bytesPerPixel = 4;
		public const int bitsPerPixel = 8;
		public const int redMask = 12;
		public const int redShift = 16;
		public const int redBits = 20;
		public const int greenMask = 24;
		public const int greenShift = 28;
		public const int greenBits = 32;
		public const int blueMask = 36;
		public const int blueShift = 40;
		public const int blueBits = 44;
		public const int width = 48;
		public const int height = 52;
		public const int pitch = 56;
		public const int supportsGfxSyscalls = 60;
	}
	public class MAAudioBufferInfo {
		public const int fmt = 0;
		public const int sampleRate = 4;
		public const int numChannels = 8;
		public const int bufferSize = 12;
		public const int buffer = 16;
	}
	public class MA_PIM_ARGS {
		public const int item = 0;
		public const int field = 4;
		public const int buf = 8;
		public const int bufSize = 12;
	}
	public class MAWidgetEventData {
		public const int eventType = 0;
		public const int widgetHandle = 4;
		public const int listItemIndex = 8;
		public const int sectionIndex = 8;
		public const int sectionItemIndex = 12;
		public const int searchBarButton = 8;
		public const int checked_ = 8;
		public const int tabIndex = 8;
		public const int status = 8;
		public const int hookType = 8;
		public const int urlData = 12;
		public const int fromScreen = 8;
		public const int toScreen = 12;
		public const int sliderValue = 8;
		public const int dayOfMonth = 8;
		public const int month = 12;
		public const int year = 16;
		public const int hour = 8;
		public const int minute = 12;
		public const int numberPickerValue = 8;
		public const int videoViewState = 8;
		public const int optionDialogButtonIndex = 8;
		public const int value = 8;
		public const int fromUser = 12;
		public const int radioGroupItemHandle = 8;
		public const int radioButtonState = 8;
		public const int optionsMenuItem = 8;
		public const int mapPinIndex = 8;
	}
	public class MA_CAMERA_FORMAT {
		public const int width = 0;
		public const int height = 4;
	}
	public class MADBBlob {
		public const int data = 0;
		public const int size = 4;
	}
	public class MADBText {
		public const int addr = 0;
		public const int length = 4;
	}
	public class MADBValue {
		public const int i = 0;
		public const int d = 0;
		public class blob {
			public const int data = 0;
			public const int size = 4;
		}
		public const int dataHandle = 0;
		public class text {
			public const int addr = 0;
			public const int length = 4;
		}
		public const int i64 = 0;
		public const int type = 8;
	}
	public class MAPushNotificationData {
		public const int type = 0;
		public const int alertMessage = 4;
		public const int alertMessageSize = 8;
		public const int soundFileName = 12;
		public const int soundFileNameSize = 16;
		public const int badgeIcon = 20;
	}
}

public class Constants {
	public const uint MoSyncHash = 0x06ebbe52;
	public const int TRANS_NONE = 0;
	public const int TRANS_ROT90 = 5;
	public const int TRANS_ROT180 = 3;
	public const int TRANS_ROT270 = 6;
	public const int TRANS_MIRROR = 2;
	public const int TRANS_MIRROR_ROT90 = 7;
	public const int TRANS_MIRROR_ROT180 = 1;
	public const int TRANS_MIRROR_ROT270 = 4;
	public const int HANDLE_SCREEN = 0;
	public const int HANDLE_LOCAL = 0;
	public const int RES_OUT_OF_MEMORY = -1;
	public const int RES_BAD_INPUT = -2;
	public const int RES_OK = 1;
	public const int MAS_CREATE_IF_NECESSARY = 1;
	public const int STERR_GENERIC = -2;
	public const int STERR_FULL = -3;
	public const int STERR_NONEXISTENT = -5;
	public const int CONNERR_GENERIC = -2;
	public const int CONNERR_MAX = -3;
	public const int CONNERR_DNS = -4;
	public const int CONNERR_INTERNAL = -5;
	public const int CONNERR_CLOSED = -6;
	public const int CONNERR_READONLY = -7;
	public const int CONNERR_FORBIDDEN = -8;
	public const int CONNERR_UNINITIALIZED = -9;
	public const int CONNERR_CONLEN = -10;
	public const int CONNERR_URL = -11;
	public const int CONNERR_UNAVAILABLE = -12;
	public const int CONNERR_CANCELED = -13;
	public const int CONNERR_PROTOCOL = -14;
	public const int CONNERR_NETWORK = -15;
	public const int CONNERR_NOHEADER = -16;
	public const int CONNERR_NOTFOUND = -17;
	public const int CONNERR_SSL = -18;
	public const int CONNERR_USER = -1000000;
	public const int CONNOP_READ = 1;
	public const int CONNOP_WRITE = 2;
	public const int CONNOP_CONNECT = 7;
	public const int CONNOP_FINISH = 11;
	public const int CONNOP_ACCEPT = 16;
	public const int CONN_MAX = 32;
	public const int BTADDR_LEN = 6;
	public const int CONN_FAMILY_INET4 = 1;
	public const int CONN_FAMILY_BT = 2;
	public const int HTTP_GET = 1;
	public const int HTTP_POST = 2;
	public const int HTTP_HEAD = 3;
	public const int HTTP_PUT = 4;
	public const int HTTP_DELETE = 5;
	public const int MA_RESOURCE_OPEN = 0x1;
	public const int MA_RESOURCE_CLOSE = 0x2;
	public const int MAK_UNKNOWN = 0;
	public const int MAK_FIRST = 0;
	public const int MAK_BACKSPACE = 8;
	public const int MAK_TAB = 9;
	public const int MAK_CLEAR = 12;
	public const int MAK_RETURN = 13;
	public const int MAK_PAUSE = 19;
	public const int MAK_ESCAPE = 27;
	public const int MAK_SPACE = 32;
	public const int MAK_EXCLAIM = 33;
	public const int MAK_QUOTEDBL = 34;
	public const int MAK_POUND = 35;
	public const int MAK_HASH = 35;
	public const int MAK_GRID = 35;
	public const int MAK_DOLLAR = 36;
	public const int MAK_AMPERSAND = 38;
	public const int MAK_QUOTE = 39;
	public const int MAK_LEFTPAREN = 40;
	public const int MAK_RIGHTPAREN = 41;
	public const int MAK_ASTERISK = 42;
	public const int MAK_STAR = 42;
	public const int MAK_PLUS = 43;
	public const int MAK_COMMA = 44;
	public const int MAK_MINUS = 45;
	public const int MAK_PERIOD = 46;
	public const int MAK_SLASH = 47;
	public const int MAK_0 = 48;
	public const int MAK_1 = 49;
	public const int MAK_2 = 50;
	public const int MAK_3 = 51;
	public const int MAK_4 = 52;
	public const int MAK_5 = 53;
	public const int MAK_6 = 54;
	public const int MAK_7 = 55;
	public const int MAK_8 = 56;
	public const int MAK_9 = 57;
	public const int MAK_COLON = 58;
	public const int MAK_SEMICOLON = 59;
	public const int MAK_LESS = 60;
	public const int MAK_EQUALS = 61;
	public const int MAK_GREATER = 62;
	public const int MAK_QUESTION = 63;
	public const int MAK_AT = 64;
	public const int MAK_LEFTBRACKET = 91;
	public const int MAK_BACKSLASH = 92;
	public const int MAK_RIGHTBRACKET = 93;
	public const int MAK_CARET = 94;
	public const int MAK_UNDERSCORE = 95;
	public const int MAK_BACKQUOTE = 96;
	public const int MAK_A = 97;
	public const int MAK_B = 98;
	public const int MAK_C = 99;
	public const int MAK_D = 100;
	public const int MAK_E = 101;
	public const int MAK_F = 102;
	public const int MAK_G = 103;
	public const int MAK_H = 104;
	public const int MAK_I = 105;
	public const int MAK_J = 106;
	public const int MAK_K = 107;
	public const int MAK_L = 108;
	public const int MAK_M = 109;
	public const int MAK_N = 110;
	public const int MAK_O = 111;
	public const int MAK_P = 112;
	public const int MAK_Q = 113;
	public const int MAK_R = 114;
	public const int MAK_S = 115;
	public const int MAK_T = 116;
	public const int MAK_U = 117;
	public const int MAK_V = 118;
	public const int MAK_W = 119;
	public const int MAK_X = 120;
	public const int MAK_Y = 121;
	public const int MAK_Z = 122;
	public const int MAK_DELETE = 127;
	public const int MAK_KP0 = 256;
	public const int MAK_KP1 = 257;
	public const int MAK_KP2 = 258;
	public const int MAK_KP3 = 259;
	public const int MAK_KP4 = 260;
	public const int MAK_KP5 = 261;
	public const int MAK_KP6 = 262;
	public const int MAK_KP7 = 263;
	public const int MAK_KP8 = 264;
	public const int MAK_KP9 = 265;
	public const int MAK_KP_PERIOD = 266;
	public const int MAK_KP_DIVIDE = 267;
	public const int MAK_KP_MULTIPLY = 268;
	public const int MAK_KP_MINUS = 269;
	public const int MAK_KP_PLUS = 270;
	public const int MAK_KP_ENTER = 271;
	public const int MAK_KP_EQUALS = 272;
	public const int MAK_UP = 273;
	public const int MAK_DOWN = 274;
	public const int MAK_RIGHT = 275;
	public const int MAK_LEFT = 276;
	public const int MAK_INSERT = 277;
	public const int MAK_HOME = 278;
	public const int MAK_END = 279;
	public const int MAK_PAGEUP = 280;
	public const int MAK_PAGEDOWN = 281;
	public const int MAK_FIRE = 284;
	public const int MAK_SOFTLEFT = 285;
	public const int MAK_SOFTRIGHT = 286;
	public const int MAK_PEN = 291;
	public const int MAK_BACK = 292;
	public const int MAK_MENU = 293;
	public const int MAK_RSHIFT = 303;
	public const int MAK_LSHIFT = 304;
	public const int MAK_RCTRL = 305;
	public const int MAK_LCTRL = 306;
	public const int MAK_RALT = 307;
	public const int MAK_LALT = 308;
	public const int MAK_SEARCH = 309;
	public const int MAKB_LEFT = 0x00001;
	public const int MAKB_UP = 0x00002;
	public const int MAKB_RIGHT = 0x00004;
	public const int MAKB_DOWN = 0x00008;
	public const int MAKB_FIRE = 0x00010;
	public const int MAKB_SOFTLEFT = 0x00020;
	public const int MAKB_SOFTRIGHT = 0x00040;
	public const int MAKB_0 = 0x00080;
	public const int MAKB_1 = 0x00100;
	public const int MAKB_2 = 0x00200;
	public const int MAKB_3 = 0x00400;
	public const int MAKB_4 = 0x00800;
	public const int MAKB_5 = 0x01000;
	public const int MAKB_6 = 0x02000;
	public const int MAKB_7 = 0x04000;
	public const int MAKB_8 = 0x08000;
	public const int MAKB_9 = 0x10000;
	public const int MAKB_ASTERISK = 0x20000;
	public const int MAKB_STAR = 0x20000;
	public const int MAKB_HASH = 0x40000;
	public const int MAKB_POUND = 0x40000;
	public const int MAKB_GRID = 0x40000;
	public const int MAKB_CLEAR = 0x80000;
	public const int EVENT_BUFFER_SIZE = 256;
	public const int EVENT_CLOSE_TIMEOUT = 2000;
	public const int EVENT_TYPE_CLOSE = 1;
	public const int EVENT_TYPE_KEY_PRESSED = 2;
	public const int EVENT_TYPE_KEY_RELEASED = 3;
	public const int EVENT_TYPE_CONN = 4;
	public const int EVENT_TYPE_BT = 5;
	public const int EVENT_TYPE_TTS = 6;
	public const int EVENT_TYPE_WLAN = 7;
	public const int EVENT_TYPE_POINTER_PRESSED = 8;
	public const int EVENT_TYPE_POINTER_RELEASED = 9;
	public const int EVENT_TYPE_POINTER_DRAGGED = 10;
	public const int EVENT_TYPE_CALL = 11;
	public const int EVENT_TYPE_FOCUS_LOST = 13;
	public const int EVENT_TYPE_FOCUS_GAINED = 14;
	public const int EVENT_TYPE_STREAM = 15;
	public const int EVENT_TYPE_LOCATION = 16;
	public const int EVENT_TYPE_LOCATION_PROVIDER = 17;
	public const int EVENT_TYPE_AUDIOBUFFER_FILL = 18;
	public const int EVENT_TYPE_AUDIO_PREPARED = 19;
	public const int EVENT_TYPE_AUDIO_COMPLETED = 20;
	public const int EVENT_TYPE_SCREEN_CHANGED = 21;
	public const int EVENT_TYPE_CHAR = 22;
	public const int EVENT_TYPE_TEXTBOX = 23;
	public const int EVENT_TYPE_HOMESCREEN_SHOWN = 24;
	public const int EVENT_TYPE_HOMESCREEN_HIDDEN = 25;
	public const int EVENT_TYPE_SCREEN_STATE_ON = 26;
	public const int EVENT_TYPE_SCREEN_STATE_OFF = 27;
	public const int EVENT_TYPE_WIDGET = 28;
	public const int EVENT_TYPE_BLUETOOTH_TURNED_OFF = 29;
	public const int EVENT_TYPE_BLUETOOTH_TURNED_ON = 30;
	public const int EVENT_TYPE_IMAGE_PICKER = 31;
	public const int EVENT_TYPE_SMS = 32;
	public const int EVENT_TYPE_SENSOR = 33;
	public const int EVENT_TYPE_ALERT = 34;
	public const int EVENT_TYPE_NFC_TAG_RECEIVED = 35;
	public const int EVENT_TYPE_NFC_TAG_DATA_READ = 36;
	public const int EVENT_TYPE_NFC_TAG_DATA_WRITTEN = 37;
	public const int EVENT_TYPE_NFC_BATCH_OP = 38;
	public const int EVENT_TYPE_NFC_TAG_AUTH_COMPLETE = 39;
	public const int EVENT_TYPE_NFC_TAG_READ_ONLY = 40;
	public const int EVENT_TYPE_OPTIONS_BOX_BUTTON_CLICKED = 41;
	public const int EVENT_TYPE_ADS_BANNER = 42;
	public const int EVENT_TYPE_LOCAL_NOTIFICATION = 43;
	public const int EVENT_TYPE_PUSH_NOTIFICATION_REGISTRATION = 44;
	public const int EVENT_TYPE_PUSH_NOTIFICATION_UNREGISTRATION = 45;
	public const int EVENT_TYPE_PUSH_NOTIFICATION = 46;
	public const int EVENT_TYPE_CAPTURE = 47;
	public const int EVENT_TYPE_PURCHASE = 48;
	public const int EVENT_TYPE_CAMERA_PREVIEW = 49;
	public const int EVENT_TYPE_NETWORK = 50;
	public const int RUNTIME_MORE = 1;
	public const int RUNTIME_JAVA = 2;
	public const int RUNTIME_SYMBIAN = 3;
	public const int RUNTIME_WINCE = 4;
	public const int REPORT_PANIC = 1;
	public const int REPORT_EXCEPTION = 2;
	public const int REPORT_PLATFORM_CODE = 3;
	public const int REPORT_USER_PANIC = 4;
	public const int REPORT_TIMEOUT = 5;
	public const int MA_EXTENSION_MODULE_UNAVAILABLE = -1;
	public const int MA_EXTENSION_FUNCTION_UNAVAILABLE = -1;
	public const int RT_PLACEHOLDER = 1;
	public const int RT_IMAGE = 2;
	public const int RT_SPRITE = 3;
	public const int RT_BINARY = 4;
	public const int RT_UBIN = 5;
	public const int RT_SKIP = 6;
	public const int RT_LABEL = 9;
	public const int RT_NIL = 10;
	public const int RT_FLUX = 127;
	public const int FONT_TYPE_SERIF = 0;
	public const int FONT_TYPE_SANS_SERIF = 1;
	public const int FONT_TYPE_MONOSPACE = 2;
	public const int FONT_STYLE_NORMAL = 0;
	public const int FONT_STYLE_BOLD = 1;
	public const int FONT_STYLE_ITALIC = 2;
	public const int RES_FONT_OK = 1;
	public const int RES_FONT_INVALID_HANDLE = -1;
	public const int RES_FONT_INDEX_OUT_OF_BOUNDS = -2;
	public const int RES_FONT_NO_TYPE_STYLE_COMBINATION = -3;
	public const int RES_FONT_NAME_NONEXISTENT = -4;
	public const int RES_FONT_LIST_NOT_INITIALIZED = -5;
	public const int RES_FONT_INSUFFICIENT_BUFFER = -6;
	public const int RES_FONT_INVALID_SIZE = -7;
	public const int RES_FONT_DELETE_DENIED = -8;
	public const int WLAN_ADDR_LEN = 6;
	public const int MA_LOC_NONE = 1;
	public const int MA_LOC_INVALID = 2;
	public const int MA_LOC_UNQUALIFIED = 3;
	public const int MA_LOC_QUALIFIED = 4;
	public const int MA_LPS_AVAILABLE = 1;
	public const int MA_LPS_TEMPORARILY_UNAVAILABLE = 2;
	public const int MA_LPS_OUT_OF_SERVICE = 3;
	public const int MA_ACCESS_READ = 1;
	public const int MA_ACCESS_READ_WRITE = 3;
	public const int MA_SEEK_SET = 0;
	public const int MA_SEEK_CUR = 1;
	public const int MA_SEEK_END = 2;
	public const int MA_FL_SORT_NONE = 0;
	public const int MA_FL_SORT_DATE = 1;
	public const int MA_FL_SORT_NAME = 2;
	public const int MA_FL_SORT_SIZE = 3;
	public const int MA_FL_ORDER_ASCENDING = 0x10000;
	public const int MA_FL_ORDER_DESCENDING = 0x20000;
	public const int MA_FERR_GENERIC = -2;
	public const int MA_FERR_NOTFOUND = -3;
	public const int MA_FERR_FORBIDDEN = -4;
	public const int MA_FERR_RENAME_FILESYSTEM = -5;
	public const int MA_FERR_RENAME_DIRECTORY = -6;
	public const int MA_FERR_WRONG_TYPE = -7;
	public const int MA_FERR_SORTING_UNSUPPORTED = -8;
	public const int MA_SMS_RESULT_SENT = 1;
	public const int MA_SMS_RESULT_NOT_SENT = 2;
	public const int MA_SMS_RESULT_DELIVERED = 3;
	public const int MA_SMS_RESULT_NOT_DELIVERED = 4;
	public const int CALLSTATE_UNKNOWN = 0;
	public const int CALLSTATE_IDLE = 1;
	public const int CALLSTATE_DIALLING = 2;
	public const int CALLSTATE_RINGING = 3;
	public const int CALLSTATE_ANSWERING = 4;
	public const int CALLSTATE_CONNECTING = 5;
	public const int CALLSTATE_CONNECTED = 6;
	public const int CALLSTATE_RECONNECTPENDING = 7;
	public const int CALLSTATE_DISCONNECTING = 8;
	public const int CALLSTATE_HOLD = 9;
	public const int CALLSTATE_TRANSFERRING = 10;
	public const int CALLSTATE_TRANSFERALERTING = 11;
	public const int STREAMEVENT_OPEN_COMPLETE = 1;
	public const int STREAMEVENT_PREPARE_COMPLETE = 2;
	public const int STREAMEVENT_PLAY_COMPLETE = 3;
	public const int STREAMEVENT_LOADING_STARTED = 4;
	public const int STREAMEVENT_LOADING_COMPLETE = 5;
	public const int STREAMEVENT_UNKNOWN_EVENT = 6;
	public const int AUDIO_FMT_S8 = 1;
	public const int AUDIO_FMT_S16 = 2;
	public const int AUDIO_FMT_U8 = 3;
	public const int AUDIO_FMT_U16 = 4;
	public const int MA_NETWORK_UNKNOWN = 0;
	public const int MA_NETWORK_NO_SERVICE = 1;
	public const int MA_NETWORK_EMERGENCY_ONLY = 2;
	public const int MA_NETWORK_SEARCHING = 3;
	public const int MA_NETWORK_BUSY = 4;
	public const int MA_NETWORK_HOME = 5;
	public const int MA_NETWORK_DENIED = 6;
	public const int MA_NETWORK_ROAMING = 7;
	public const int MA_IAP_METHOD_STANDARD = 1;
	public const int MA_IAP_METHOD_WLAN = 2;
	public const int MA_IAP_FILTER_NOT_WLAN = 1;
	public const int MA_IAP_FILTER_WLAN = 2;
	public const int MA_IAP_FILTER_ALL = 3;
	public const int MA_PIM_CONTACTS = 1;
	public const int MA_PIM_EVENTS = 2;
	public const int MA_PIM_TODOS = 3;
	public const int MA_PIM_TYPE_BINARY = 0;
	public const int MA_PIM_TYPE_BOOLEAN = 1;
	public const int MA_PIM_TYPE_DATE = 2;
	public const int MA_PIM_TYPE_INT = 3;
	public const int MA_PIM_TYPE_STRING = 4;
	public const int MA_PIM_TYPE_STRING_ARRAY = 5;
	public const int MA_PIM_FIELD_CONTACT_ADDR = 100;
	public const int MA_PIM_FIELD_CONTACT_BIRTHDAY = 101;
	public const int MA_PIM_FIELD_CONTACT_CLASS = 102;
	public const int MA_PIM_FIELD_CONTACT_EMAIL = 103;
	public const int MA_PIM_FIELD_CONTACT_FORMATTED_ADDR = 104;
	public const int MA_PIM_FIELD_CONTACT_FORMATTED_NAME = 105;
	public const int MA_PIM_FIELD_CONTACT_NAME = 106;
	public const int MA_PIM_FIELD_CONTACT_NICKNAME = 107;
	public const int MA_PIM_FIELD_CONTACT_NOTE = 108;
	public const int MA_PIM_FIELD_CONTACT_ORG = 109;
	public const int MA_PIM_FIELD_CONTACT_PHOTO = 110;
	public const int MA_PIM_FIELD_CONTACT_PHOTO_URL = 111;
	public const int MA_PIM_FIELD_CONTACT_PUBLIC_KEY = 112;
	public const int MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING = 113;
	public const int MA_PIM_FIELD_CONTACT_REVISION = 114;
	public const int MA_PIM_FIELD_CONTACT_TEL = 115;
	public const int MA_PIM_FIELD_CONTACT_TITLE = 116;
	public const int MA_PIM_FIELD_CONTACT_UID = 117;
	public const int MA_PIM_FIELD_CONTACT_URL = 118;
	public const int MA_PIM_FIELD_CONTACT_IM = 119;
	public const int MA_PIM_FIELD_CONTACT_RELATION = 120;
	public const int MA_PIM_FIELD_CONTACT_ORG_INFO = 121;
	public const int MA_PIM_CONTACT_ADDR_POBOX = 0;
	public const int MA_PIM_CONTACT_ADDR_EXTRA = 1;
	public const int MA_PIM_CONTACT_ADDR_STREET = 2;
	public const int MA_PIM_CONTACT_ADDR_LOCALITY = 3;
	public const int MA_PIM_CONTACT_ADDR_REGION = 4;
	public const int MA_PIM_CONTACT_ADDR_POSTALCODE = 5;
	public const int MA_PIM_CONTACT_ADDR_COUNTRY = 6;
	public const int MA_PIM_CONTACT_ADDR_NEIGHBORHOOD = 7;
	public const int MA_PIM_CONTACT_NAME_FAMILY = 0;
	public const int MA_PIM_CONTACT_NAME_GIVEN = 1;
	public const int MA_PIM_CONTACT_NAME_OTHER = 2;
	public const int MA_PIM_CONTACT_NAME_PREFIX = 3;
	public const int MA_PIM_CONTACT_NAME_SUFFIX = 4;
	public const int MA_PIM_CONTACT_NAME_PHONETIC_FAMILY = 5;
	public const int MA_PIM_CONTACT_NAME_PHONETIC_GIVEN = 6;
	public const int MA_PIM_CONTACT_NAME_PHONETIC_OTHER = 7;
	public const int MA_PIM_CONTACT_ORG_INFO_DEPARTMENT = 0;
	public const int MA_PIM_CONTACT_ORG_INFO_JOB_DESCRIPTION = 1;
	public const int MA_PIM_CONTACT_ORG_INFO_SYMBOL = 2;
	public const int MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME = 3;
	public const int MA_PIM_CONTACT_ORG_INFO_OFFICE_LOCATION = 4;
	public const int MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME_STYLE = 5;
	public const int MA_PIM_CONTACT_IM_USERNAME = 0;
	public const int MA_PIM_CONTACT_IM_PROTOCOL = 1;
	public const int MA_PIM_CONTACT_CLASS_CONFIDENTIAL = 200;
	public const int MA_PIM_CONTACT_CLASS_PRIVATE = 201;
	public const int MA_PIM_CONTACT_CLASS_PUBLIC = 202;
	public const String MA_PIM_CONTACT_IM_PROTOCOL_AIM = "AIM";
	public const String MA_PIM_CONTACT_IM_PROTOCOL_MSN = "MSN";
	public const String MA_PIM_CONTACT_IM_PROTOCOL_YAHOO = "Yahoo";
	public const String MA_PIM_CONTACT_IM_PROTOCOL_SKYPE = "Skype";
	public const String MA_PIM_CONTACT_IM_PROTOCOL_QQ = "QQ";
	public const String MA_PIM_CONTACT_IM_PROTOCOL_GOOGLE_TALK = "GoogleTalk";
	public const String MA_PIM_CONTACT_IM_PROTOCOL_ICQ = "ICQ";
	public const String MA_PIM_CONTACT_IM_PROTOCOL_JABBER = "Jabber";
	public const String MA_PIM_CONTACT_IM_PROTOCOL_NETMEETING = "Netmeeting";
	public const int MA_PIM_ATTR_ASST = 1;
	public const int MA_PIM_ATTR_AUTO = 2;
	public const int MA_PIM_ATTR_FAX = 4;
	public const int MA_PIM_ATTR_HOME = 8;
	public const int MA_PIM_ATTR_MOBILE = 16;
	public const int MA_PIM_ATTR_OTHER = 32;
	public const int MA_PIM_ATTR_PAGER = 64;
	public const int MA_PIM_ATTR_PREFERRED = 128;
	public const int MA_PIM_ATTR_SMS = 256;
	public const int MA_PIM_ATTR_WORK = 512;
	public const int MA_PIM_ATTR_ADDR_HOME = 101;
	public const int MA_PIM_ATTR_ADDR_WORK = 102;
	public const int MA_PIM_ATTR_ADDR_OTHER = 103;
	public const int MA_PIM_ATTR_ADDR_CUSTOM = 104;
	public const int MA_PIM_ATTR_EMAIL_HOME = 201;
	public const int MA_PIM_ATTR_EMAIL_WORK = 202;
	public const int MA_PIM_ATTR_EMAIL_MOBILE = 203;
	public const int MA_PIM_ATTR_EMAIL_OTHER = 204;
	public const int MA_PIM_ATTR_EMAIL_CUSTOM = 205;
	public const int MA_PIM_ATTR_FORMATTED_ADDR_HOME = 301;
	public const int MA_PIM_ATTR_FORMATTED_ADDR_WORK = 302;
	public const int MA_PIM_ATTR_FORMATTED_ADDR_OTHER = 303;
	public const int MA_PIM_ATTR_FORMATTED_ADDR_CUSTOM = 304;
	public const int MA_PIM_ATTR_ORG_WORK = 401;
	public const int MA_PIM_ATTR_ORG_OTHER = 402;
	public const int MA_PIM_ATTR_ORG_CUSTOM = 403;
	public const int MA_PIM_ATTR_PHONE_HOME = 501;
	public const int MA_PIM_ATTR_PHONE_MOBILE = 502;
	public const int MA_PIM_ATTR_PHONE_HOME_FAX = 503;
	public const int MA_PIM_ATTR_PHONE_WORK_FAX = 504;
	public const int MA_PIM_ATTR_PHONE_PAGER = 505;
	public const int MA_PIM_ATTR_PHONE_IPHONE = 506;
	public const int MA_PIM_ATTR_PHONE_WORK = 507;
	public const int MA_PIM_ATTR_PHONE_CALLBACK = 508;
	public const int MA_PIM_ATTR_PHONE_CAR = 509;
	public const int MA_PIM_ATTR_PHONE_COMPANY_MAIN = 510;
	public const int MA_PIM_ATTR_PHONE_ISDN = 511;
	public const int MA_PIM_ATTR_PHONE_OTHER_FAX = 512;
	public const int MA_PIM_ATTR_PHONE_RADIO = 513;
	public const int MA_PIM_ATTR_PHONE_TELEX = 514;
	public const int MA_PIM_ATTR_PHONE_TTY_TDD = 515;
	public const int MA_PIM_ATTR_PHONE_WORK_MOBILE = 516;
	public const int MA_PIM_ATTR_PHONE_WORK_PAGER = 517;
	public const int MA_PIM_ATTR_PHONE_ASSISTANT = 518;
	public const int MA_PIM_ATTR_PHONE_MMS = 519;
	public const int MA_PIM_ATTR_PHONE_OTHER = 520;
	public const int MA_PIM_ATTR_PHONE_CUSTOM = 521;
	public const int MA_PIM_ATTR_TITLE_WORK = 601;
	public const int MA_PIM_ATTR_TITLE_OTHER = 602;
	public const int MA_PIM_ATTR_TITLE_CUSTOM = 603;
	public const int MA_PIM_ATTR_WEBSITE_HOMEPAGE = 701;
	public const int MA_PIM_ATTR_WEBSITE_BLOG = 702;
	public const int MA_PIM_ATTR_WEBSITE_PROFILE = 703;
	public const int MA_PIM_ATTR_WEBSITE_HOME = 704;
	public const int MA_PIM_ATTR_WEBSITE_WORK = 705;
	public const int MA_PIM_ATTR_WEBSITE_FTP = 706;
	public const int MA_PIM_ATTR_WEBSITE_OTHER = 707;
	public const int MA_PIM_ATTR_WEBSITE_CUSTOM = 708;
	public const int MA_PIM_ATTR_IM_HOME = 801;
	public const int MA_PIM_ATTR_IM_WORK = 802;
	public const int MA_PIM_ATTR_IM_OTHER = 803;
	public const int MA_PIM_ATTR_IM_CUSTOM = 804;
	public const int MA_PIM_ATTR_RELATION_MOTHER = 901;
	public const int MA_PIM_ATTR_RELATION_FATHER = 902;
	public const int MA_PIM_ATTR_RELATION_PARENT = 903;
	public const int MA_PIM_ATTR_RELATION_SISTER = 904;
	public const int MA_PIM_ATTR_RELATION_BROTHER = 905;
	public const int MA_PIM_ATTR_RELATION_CHILD = 906;
	public const int MA_PIM_ATTR_RELATION_FRIEND = 907;
	public const int MA_PIM_ATTR_RELATION_SPOUSE = 908;
	public const int MA_PIM_ATTR_RELATION_PARTNER = 909;
	public const int MA_PIM_ATTR_RELATION_MANAGER = 910;
	public const int MA_PIM_ATTR_RELATION_ASSISTANT = 911;
	public const int MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER = 912;
	public const int MA_PIM_ATTR_RELATION_REFERRED_BY = 913;
	public const int MA_PIM_ATTR_RELATION_RELATIVE = 914;
	public const int MA_PIM_ATTR_RELATION_CUSTOM = 915;
	public const int MA_PIM_ATTR_ORG_INFO_WORK = 1001;
	public const int MA_PIM_ATTR_ORG_INFO_OTHER = 1002;
	public const int MA_PIM_ATTR_ORG_INFO_CUSTOM = 1003;
	public const int MA_PIM_ATTRPREFERRED = 0x10000;
	public const int MA_PIM_ERR_NONE = 0;
	public const int MA_PIM_ERR_UNAVAILABLE = -1;
	public const int MA_PIM_ERR_NATIVE_TYPE_MISMATCH = -2;
	public const int MA_PIM_ERR_LIST_TYPE_INVALID = -3;
	public const int MA_PIM_ERR_LIST_UNAVAILABLE = -4;
	public const int MA_PIM_ERR_LIST_ALREADY_OPENED = -5;
	public const int MA_PIM_ERR_FIELD_INVALID = -6;
	public const int MA_PIM_ERR_FIELD_UNSUPPORTED = -7;
	public const int MA_PIM_ERR_FIELD_EMPTY = -8;
	public const int MA_PIM_ERR_FIELD_COUNT_MAX = -9;
	public const int MA_PIM_ERR_FIELD_READ_ONLY = -10;
	public const int MA_PIM_ERR_FIELD_WRITE_ONLY = -11;
	public const int MA_PIM_ERR_INDEX_INVALID = -12;
	public const int MA_PIM_ERR_HANDLE_INVALID = -13;
	public const int MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED = -14;
	public const int MA_PIM_ERR_ATTRIBUTE_CUSTOM = -15;
	public const int MA_PIM_ERR_BUFFER_INVALID = -16;
	public const int MA_PIM_ERR_NO_LABEL = -17;
	public const int MA_PIM_ERR_OPERATION_NOT_PERMITTED = -18;
	public const String MAW_ACTIVITY_INDICATOR_IN_PROGRESS = "inProgress";
	public const String MAW_BUTTON_TEXT = "text";
	public const String MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT = "textVerticalAlignment";
	public const String MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT = "textHorizontalAlignment";
	public const String MAW_BUTTON_FONT_COLOR = "fontColor";
	public const String MAW_BUTTON_FONT_SIZE = "fontSize";
	public const String MAW_BUTTON_FONT_HANDLE = "fontHandle";
	public const String MAW_CHECK_BOX_CHECKED = "checked";
	public const String MAW_DATE_PICKER_MAX_DATE = "maxDate";
	public const String MAW_DATE_PICKER_MAX_DATE_YEAR = "maxdateyear";
	public const String MAW_DATE_PICKER_MAX_DATE_MONTH = "maxdatemonth";
	public const String MAW_DATE_PICKER_MAX_DATE_DAY = "maxdateday";
	public const String MAW_DATE_PICKER_MIN_DATE = "minDate";
	public const String MAW_DATE_PICKER_MIN_DATE_YEAR = "mindateyear";
	public const String MAW_DATE_PICKER_MIN_DATE_MONTH = "mindatemonth";
	public const String MAW_DATE_PICKER_MIN_DATE_DAY = "mindateday";
	public const String MAW_DATE_PICKER_YEAR = "year";
	public const String MAW_DATE_PICKER_MONTH = "month";
	public const String MAW_DATE_PICKER_DAY_OF_MONTH = "dayOfMonth";
	public const int MAW_EDIT_BOX_MODE_SINGLE_LINE = 0;
	public const int MAW_EDIT_BOX_MODE_MULTI_LINE = 1;
	public const int MAW_EDIT_BOX_TYPE_ANY = 0;
	public const int MAW_EDIT_BOX_TYPE_EMAILADDR = 1;
	public const int MAW_EDIT_BOX_TYPE_NUMERIC = 2;
	public const int MAW_EDIT_BOX_TYPE_PHONENUMBER = 3;
	public const int MAW_EDIT_BOX_TYPE_URL = 4;
	public const int MAW_EDIT_BOX_TYPE_DECIMAL = 5;
	public const int MAW_EDIT_BOX_TYPE_SINGLE_LINE = 6;
	public const int MAW_EDIT_BOX_FLAG_PASSWORD = 0;
	public const int MAW_EDIT_BOX_FLAG_SENSITIVE = 1;
	public const int MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD = 2;
	public const int MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE = 3;
	public const int MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS = 4;
	public const int MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DEFAULT = 0;
	public const int MAW_EDIT_BOX_AUTOCORRECTION_TYPE_ENABLE = 1;
	public const int MAW_EDIT_BOX_AUTOCORRECTION_TYPE_DISABLE = 2;
	public const String MAW_EDIT_BOX_TEXT = "text";
	public const String MAW_EDIT_BOX_PLACEHOLDER = "placeholder";
	public const String MAW_EDIT_BOX_SHOW_KEYBOARD = "showKeyboard";
	public const String MAW_EDIT_BOX_EDIT_MODE = "editMode";
	public const String MAW_EDIT_BOX_INPUT_MODE = "inputMode";
	public const String MAW_EDIT_BOX_INPUT_FLAG = "inputFlag";
	public const String MAW_EDIT_BOX_FONT_COLOR = "fontColor";
	public const String MAW_EDIT_BOX_LINES_NUMBER = "linesNumber";
	public const String MAW_EDIT_BOX_MAX_LINES = "maxLines";
	public const String MAW_EDIT_BOX_MIN_LINES = "minLines";
	public const String MAW_EDIT_BOX_MAX_LENGTH = "maxLength";
	public const String MAW_EDIT_BOX_PLACEHOLDER_FONT_COLOR = "placeholderFontColor";
	public const String MAW_EDIT_BOX_MODE = "mode";
	public const String MAW_EDIT_BOX_AUTOCORRECTION_TYPE = "autoCorrectionType";
	public const String MAW_EDIT_BOX_TEXT_HORIZONTAL_ALIGNMENT = "textHorizontalAlignment";
	public const String MAW_GL_VIEW_INVALIDATE = "invalidate";
	public const String MAW_GL_VIEW_BIND = "bind";
	public const String MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT = "childVerticalAlignment";
	public const String MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT = "childHorizontalAlignment";
	public const String MAW_HORIZONTAL_LAYOUT_PADDING_TOP = "paddingTop";
	public const String MAW_HORIZONTAL_LAYOUT_PADDING_LEFT = "paddingLeft";
	public const String MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT = "paddingRight";
	public const String MAW_HORIZONTAL_LAYOUT_PADDING_BOTTOM = "paddingBottom";
	public const String MAW_IMAGE_IMAGE = "image";
	public const String MAW_IMAGE_SCALE_MODE = "scaleMode";
	public const String MAW_IMAGE_BUTTON_TEXT = "text";
	public const String MAW_IMAGE_BUTTON_TEXT_VERTICAL_ALIGNMENT = "textVerticalAlignment";
	public const String MAW_IMAGE_BUTTON_TEXT_HORIZONTAL_ALIGNMENT = "textHorizontalAlignment";
	public const String MAW_IMAGE_BUTTON_FONT_COLOR = "fontColor";
	public const String MAW_IMAGE_BUTTON_FONT_SIZE = "fontSize";
	public const String MAW_IMAGE_BUTTON_BACKGROUND_IMAGE = "backgroundImage";
	public const String MAW_IMAGE_BUTTON_IMAGE = "image";
	public const String MAW_IMAGE_BUTTON_FONT_HANDLE = "fontHandle";
	public const String MAW_LABEL_TEXT = "text";
	public const String MAW_LABEL_TEXT_VERTICAL_ALIGNMENT = "textVerticalAlignment";
	public const String MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT = "textHorizontalAlignment";
	public const String MAW_LABEL_FONT_COLOR = "fontColor";
	public const String MAW_LABEL_FONT_SIZE = "fontSize";
	public const String MAW_LABEL_FONT_HANDLE = "fontHandle";
	public const String MAW_LABEL_MAX_NUMBER_OF_LINES = "maxNumberOfLines";
	public const int MAW_LIST_VIEW_TYPE_DEFAULT = 0;
	public const int MAW_LIST_VIEW_TYPE_ALPHABETICAL = 1;
	public const int MAW_LIST_VIEW_TYPE_SEGMENTED = 2;
	public const int MAW_LIST_VIEW_MODE_DISPLAY = 0;
	public const int MAW_LIST_VIEW_MODE_EDIT = 1;
	public const int MAW_LIST_VIEW_STYLE_SUBTITLE = 0;
	public const int MAW_LIST_VIEW_STYLE_NO_SUBTITLE = 1;
	public const String MAW_LIST_VIEW_TYPE = "type";
	public const String MAW_LIST_VIEW_STYLE = "style";
	public const String MAW_LIST_VIEW_MODE = "mode";
	public const String MAW_LIST_VIEW_RELOAD_DATA = "reload_data";
	public const String MAW_LIST_VIEW_ALLOW_SELECTION = "allowselection";
	public const String MAW_LIST_VIEW_REQUEST_FOCUS = "requestFocus";
	public const int MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE = 0;
	public const int MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE = 1;
	public const int MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL = 2;
	public const int MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK = 3;
	public const int MAW_LIST_VIEW_ITEM_EDIT_STYLE_NONE = 0;
	public const int MAW_LIST_VIEW_ITEM_EDIT_STYLE_DELETE = 1;
	public const int MAW_LIST_VIEW_ITEM_EDIT_STYLE_INSERT = 2;
	public const int MAW_LIST_VIEW_ITEM_SELECTION_STYLE_NONE = 0;
	public const int MAW_LIST_VIEW_ITEM_SELECTION_STYLE_BLUE = 1;
	public const int MAW_LIST_VIEW_ITEM_SELECTION_STYLE_GRAY = 2;
	public const String MAW_LIST_VIEW_ITEM_TEXT = "text";
	public const String MAW_LIST_VIEW_ITEM_SUBTITLE = "subtitle";
	public const String MAW_LIST_VIEW_ITEM_ICON = "icon";
	public const String MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE = "accessoryType";
	public const String MAW_LIST_VIEW_ITEM_FONT_COLOR = "fontColor";
	public const String MAW_LIST_VIEW_ITEM_FONT_SIZE = "fontSize";
	public const String MAW_LIST_VIEW_ITEM_FONT_HANDLE = "fontHandle";
	public const String MAW_LIST_VIEW_ITEM_EDIT = "edit";
	public const String MAW_LIST_VIEW_ITEM_DELETE_BUTTON_TITLE = "deleteButtonTitle";
	public const String MAW_LIST_VIEW_ITEM_IS_SHOWING_DELETE_CONFIRMATION = "isShowingDeleteConfirmation";
	public const String MAW_LIST_VIEW_ITEM_SET_SELECTED = "setSelected";
	public const String MAW_LIST_VIEW_ITEM_SET_UNSELECTED = "setUnselected";
	public const String MAW_LIST_VIEW_ITEM_IS_SELECTED = "isSelected";
	public const String MAW_LIST_VIEW_ITEM_SET_HIGHLIGHTED = "setHighlighted";
	public const String MAW_LIST_VIEW_ITEM_SET_UNHIGHLIGHTED = "setUnhighlighted";
	public const String MAW_LIST_VIEW_ITEM_IS_HIGHLIGHTED = "isHighlighted";
	public const String MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_INT = "accessoryTypeInt";
	public const String MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_EDIT = "accessoryTypeEditState";
	public const String MAW_LIST_VIEW_ITEM_EDITING_STYLE = "editingStyle";
	public const String MAW_LIST_VIEW_ITEM_SELECTION_STYLE = "selectionStyle";
	public const int MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL = 1;
	public const int MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED = 2;
	public const String MAW_LIST_VIEW_SECTION_TYPE = "type";
	public const String MAW_LIST_VIEW_SECTION_TITLE = "title";
	public const String MAW_LIST_VIEW_SECTION_HEADER = "header";
	public const String MAW_LIST_VIEW_SECTION_FOOTER = "footer";
	public const String MAW_LIST_VIEW_SECTION_HEADER_BACKGROUND = "headerBackground";
	public const String MAW_LIST_VIEW_SECTION_FOOTER_BACKGROUND = "footerBackground";
	public const String MAW_LIST_VIEW_SECTION_HEADER_FONT_COLOR = "headerFontColor";
	public const String MAW_LIST_VIEW_SECTION_FOOTER_FONT_COLOR = "footerFontColor";
	public const String MAW_LIST_VIEW_SECTION_HEADER_FONT_SIZE = "headerFontSize";
	public const String MAW_LIST_VIEW_SECTION_FOOTER_FONT_SIZE = "footerFontSize";
	public const String MAW_LIST_VIEW_SECTION_HEADER_FONT_HANDLE = "headerFontHandle";
	public const String MAW_LIST_VIEW_SECTION_FOOTER_FONT_HANDLE = "footerFontHandle";
	public const String MAW_LIST_VIEW_SECTION_HEADER_HORIZONTAL_ALIGNMENT = "headerHorizontalAlignment";
	public const String MAW_LIST_VIEW_SECTION_FOOTER_HORIZONTAL_ALIGNMENT = "footerHorizontalAlignment";
	public const String MAW_LIST_VIEW_SECTION_HEADER_VERTICAL_ALIGNMENT = "headerVerticalAlignment";
	public const String MAW_LIST_VIEW_SECTION_FOOTER_VERTICAL_ALIGNMENT = "footerVerticalAlignment";
	public const int MAW_MAP_TYPE_ROAD = 0;
	public const int MAW_MAP_TYPE_SATELLITE = 1;
	public const String MAW_MAP_API_KEY_GOOGLE = "api_key_google";
	public const String MAW_MAP_API_KEY_BING = "api_key_bing";
	public const String MAW_MAP_TYPE = "type";
	public const String MAW_MAP_ZOOM_LEVEL = "zoom_level";
	public const String MAW_MAP_INTERRACTION_ENABLED = "interraction_enabled";
	public const String MAW_MAP_CENTER_LATITUDE = "center_latitude";
	public const String MAW_MAP_CENTER_LONGITUDE = "center_longitude";
	public const String MAW_MAP_CENTER_ZOOM_LEVEL = "center_zoom_level";
	public const String MAW_MAP_CENTERED = "centered";
	public const String MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LATITUDE = "visible_area_upper_left_corner_latitude";
	public const String MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LONGITUDE = "visible_area_upper_left_corner_longitude";
	public const String MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LATITUDE = "visible_area_lower_right_corner_latitude";
	public const String MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LONGITUDE = "visible_area_lower_right_corner_longitude";
	public const String MAW_MAP_CENTERED_ON_VISIBLE_AREA = "centered_on_visible_area";
	public const String MAW_MAP_PIN_LATITUDE = "latitude";
	public const String MAW_MAP_PIN_LONGITUDE = "longitude";
	public const String MAW_MAP_PIN_TEXT = "text";
	public const String MAW_MODAL_DIALOG_TITLE = "title";
	public const String MAW_MODAL_DIALOG_ARROW_POSITION = "arrowPosition";
	public const String MAW_MODAL_DIALOG_USER_CAN_DISMISS = "userCanDismiss";
	public const String MAW_NAV_BAR_TITLE = "title";
	public const String MAW_NAV_BAR_ICON = "icon";
	public const String MAW_NAV_BAR_BACK_BTN = "backBtn";
	public const String MAW_NAV_BAR_TITLE_FONT_COLOR = "titleFontColor";
	public const String MAW_NAV_BAR_TITLE_FONT_SIZE = "titleFontSize";
	public const String MAW_NAV_BAR_TITLE_FONT_HANDLE = "titleFontHandle";
	public const String MAW_NUMBER_PICKER_VALUE = "value";
	public const String MAW_NUMBER_PICKER_MIN_VALUE = "minValue";
	public const String MAW_NUMBER_PICKER_MAX_VALUE = "maxValue";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_ADD = "17301555";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_AGENDA = "17301556";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_ALWAYS_LANDSCAPE_PORTRAIT = "17301557";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_CALL = "17301558";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_CAMERA = "17301559";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_CLOSE_CLEAR_CANCEL = "17301560";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_COMPASS = "17301561";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_CROP = "17301562";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_DAY = "17301563";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_DELETE = "17301564";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_DIRECTIONS = "17301565";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_EDIT = "17301566";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_GALLERY = "17301567";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_HELP = "17301568";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_INFO_DETAILS = "17301569";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_MANAGE = "17301570";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_MAPMODE = "17301571";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_MONTH = "17301572";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_MORE = "17301573";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_MY_CALENDAR = "17301574";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_MYLOCATION = "17301575";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_MYPLACES = "17301576";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_PREFERENCES = "17301577";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_RECENT_HISTORY = "17301578";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_REPORT_IMAGE = "17301579";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_REVERT = "17301580";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_ROTATE = "17301581";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_SAVE = "17301582";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_SEARCH = "17301583";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_SEND = "17301584";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_SET_AS = "17301585";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_SHARE = "17301586";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_SLIDESHOW = "17301587";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_SORT_ALPHABETICALLY = "17301660";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_SORT_BY_SIZE = "17301661";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_TODAY = "17301588";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_UPLOAD = "17301589";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_UPLOAD_YOU_TUBE = "17301590";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_VIEW = "17301591";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_WEEK = "17301592";
	public const String MAW_OPTIONS_MENU_ICON_CONSTANT_ZOOM = "17301593";
	public const String MAW_PANORAMA_VIEW_TITLE = "title";
	public const String MAW_PANORAMA_VIEW_CURRENT_SCREEN = "currentScreen";
	public const String MAW_PANORAMA_VIEW_BACKGROUND_IMAGE = "backgroundImage";
	public const String MAW_PROGRESS_BAR_MAX = "max";
	public const String MAW_PROGRESS_BAR_PROGRESS = "progress";
	public const String MAW_PROGRESS_BAR_INCREMENT_PROGRESS = "incrementProgress";
	public const String MAW_RADIO_BUTTON_CHECKED = "checked";
	public const String MAW_RADIO_BUTTON_TOGGLE = "toggle";
	public const String MAW_RADIO_BUTTON_TEXT = "text";
	public const String MAW_RADIO_BUTTON_TEXT_COLOR = "textcolor";
	public const String MAW_RADIO_GROUP_ADD_VIEW = "addview";
	public const String MAW_RADIO_GROUP_CLEAR_CHECK = "clearcheck";
	public const String MAW_RADIO_GROUP_SELECTED = "selected";
	public const String MAW_RATING_BAR_NUM_STAR = "numstar";
	public const String MAW_RATING_BAR_RATING = "rating";
	public const String MAW_RATING_BAR_GRANULARITY = "granularity";
	public const String MAW_RELATIVE_LAYOUT_SCROLLABLE = "scrollable";
	public const String MAW_RELATIVE_LAYOUT_CONTENT_OFFSET = "contentOffset";
	public const String MAW_SCREEN_TITLE = "title";
	public const String MAW_SCREEN_ICON = "icon";
	public const String MAW_SCREEN_REMOVE_OPTIONS_MENU = "removeoptionsmenu";
	public const String MAW_SEARCH_BAR_TEXT = "text";
	public const String MAW_SEARCH_BAR_PLACEHOLDER = "placeholder";
	public const String MAW_SEARCH_BAR_SHOW_KEYBOARD = "showKeyboard";
	public const String MAW_SLIDER_MAX = "max";
	public const String MAW_SLIDER_VALUE = "value";
	public const String MAW_SLIDER_INCREASE_VALUE = "increaseValue";
	public const String MAW_SLIDER_DECREASE_VALUE = "decreaseValue";
	public const String MAW_STACK_SCREEN_TITLE = "title";
	public const String MAW_STACK_SCREEN_ICON = "icon";
	public const String MAW_STACK_SCREEN_BACK_BUTTON_ENABLED = "backButtonEnabled";
	public const String MAW_TAB_SCREEN_TITLE = "title";
	public const String MAW_TAB_SCREEN_ICON = "icon";
	public const String MAW_TAB_SCREEN_CURRENT_TAB = "currentTab";
	public const String MAW_TIME_PICKER_CURRENT_HOUR = "currentHour";
	public const String MAW_TIME_PICKER_CURRENT_MINUTE = "currentMinute";
	public const String MAW_TOGGLE_BUTTON_CHECKED = "checked";
	public const String MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT = "childVerticalAlignment";
	public const String MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT = "childHorizontalAlignment";
	public const String MAW_VERTICAL_LAYOUT_PADDING_TOP = "paddingTop";
	public const String MAW_VERTICAL_LAYOUT_PADDING_LEFT = "paddingLeft";
	public const String MAW_VERTICAL_LAYOUT_PADDING_RIGHT = "paddingRight";
	public const String MAW_VERTICAL_LAYOUT_PADDING_BOTTOM = "paddingBottom";
	public const String MAW_VERTICAL_LAYOUT_SCROLLABLE = "scrollable";
	public const int MAW_VIDEO_VIEW_ACTION_PLAY = 1;
	public const int MAW_VIDEO_VIEW_ACTION_PAUSE = 2;
	public const int MAW_VIDEO_VIEW_ACTION_STOP = 3;
	public const int MAW_VIDEO_VIEW_STATE_PLAYING = 1;
	public const int MAW_VIDEO_VIEW_STATE_PAUSED = 2;
	public const int MAW_VIDEO_VIEW_STATE_STOPPED = 3;
	public const int MAW_VIDEO_VIEW_STATE_FINISHED = 4;
	public const int MAW_VIDEO_VIEW_STATE_SOURCE_READY = 5;
	public const int MAW_VIDEO_VIEW_STATE_INTERRUPTED = 6;
	public const String MAW_VIDEO_VIEW_PATH = "path";
	public const String MAW_VIDEO_VIEW_URL = "url";
	public const String MAW_VIDEO_VIEW_ACTION = "action";
	public const String MAW_VIDEO_VIEW_SEEK_TO = "seekTo";
	public const String MAW_VIDEO_VIEW_DURATION = "duration";
	public const String MAW_VIDEO_VIEW_BUFFER_PERCENTAGE = "bufferPercentage";
	public const String MAW_VIDEO_VIEW_CURRENT_POSITION = "currentPosition";
	public const String MAW_VIDEO_VIEW_CONTROL = "control";
	public const String MAW_WEB_VIEW_URL = "url";
	public const String MAW_WEB_VIEW_HTML = "html";
	public const String MAW_WEB_VIEW_BASE_URL = "baseUrl";
	public const String MAW_WEB_VIEW_SOFT_HOOK = "softHook";
	public const String MAW_WEB_VIEW_HARD_HOOK = "hardHook";
	public const String MAW_WEB_VIEW_NEW_URL = "newurl";
	public const String MAW_WEB_VIEW_HORIZONTAL_SCROLL_BAR_ENABLED = "horizontalScrollBarEnabled";
	public const String MAW_WEB_VIEW_VERTICAL_SCROLL_BAR_ENABLED = "verticalScrollBarEnabled";
	public const String MAW_WEB_VIEW_ENABLE_ZOOM = "enableZoom";
	public const String MAW_WEB_VIEW_NAVIGATE = "navigate";
	public const String MAW_ALIGNMENT_LEFT = "left";
	public const String MAW_ALIGNMENT_RIGHT = "right";
	public const String MAW_ALIGNMENT_CENTER = "center";
	public const String MAW_ALIGNMENT_TOP = "top";
	public const String MAW_ALIGNMENT_BOTTOM = "bottom";
	public const int MAW_CONSTANT_MOSYNC_SCREEN_HANDLE = 0;
	public const int MAW_CONSTANT_FILL_AVAILABLE_SPACE = -1;
	public const int MAW_CONSTANT_WRAP_CONTENT = -2;
	public const int MAW_CONSTANT_STARTED = 1;
	public const int MAW_CONSTANT_DONE = 2;
	public const int MAW_CONSTANT_STOPPED = 3;
	public const int MAW_CONSTANT_ERROR = -1;
	public const int MAW_CONSTANT_SOFT = 5;
	public const int MAW_CONSTANT_HARD = 6;
	public const int MAW_CONSTANT_MESSAGE = 7;
	public const int MAW_CONSTANT_ARROW_UP = 1;
	public const int MAW_CONSTANT_ARROW_DOWN = 2;
	public const int MAW_CONSTANT_ARROW_LEFT = 4;
	public const int MAW_CONSTANT_ARROW_RIGHT = 8;
	public const int MAW_CONSTANT_ARROW_ANY = 15;
	public const int MAW_EVENT_POINTER_PRESSED = 2;
	public const int MAW_EVENT_POINTER_RELEASED = 3;
	public const int MAW_EVENT_CONTENT_LOADED = 4;
	public const int MAW_EVENT_CLICKED = 5;
	public const int MAW_EVENT_ITEM_CLICKED = 6;
	public const int MAW_EVENT_TAB_CHANGED = 7;
	public const int MAW_EVENT_GL_VIEW_READY = 8;
	public const int MAW_EVENT_WEB_VIEW_URL_CHANGED = 9;
	public const int MAW_EVENT_STACK_SCREEN_POPPED = 10;
	public const int MAW_EVENT_SLIDER_VALUE_CHANGED = 11;
	public const int MAW_EVENT_DATE_PICKER_VALUE_CHANGED = 12;
	public const int MAW_EVENT_TIME_PICKER_VALUE_CHANGED = 13;
	public const int MAW_EVENT_NUMBER_PICKER_VALUE_CHANGED = 14;
	public const int MAW_EVENT_VIDEO_STATE_CHANGED = 15;
	public const int MAW_EVENT_EDIT_BOX_EDITING_DID_BEGIN = 16;
	public const int MAW_EVENT_EDIT_BOX_EDITING_DID_END = 17;
	public const int MAW_EVENT_EDIT_BOX_TEXT_CHANGED = 18;
	public const int MAW_EVENT_EDIT_BOX_RETURN = 19;
	public const int MAW_EVENT_WEB_VIEW_CONTENT_LOADING = 20;
	public const int MAW_EVENT_WEB_VIEW_HOOK_INVOKED = 21;
	public const int MAW_EVENT_DIALOG_DISMISSED = 22;
	public const int MAW_EVENT_SCREEN_ORIENTATION_WILL_CHANGE = 23;
	public const int MAW_EVENT_SCREEN_ORIENTATION_DID_CHANGE = 24;
	public const int MAW_EVENT_RATING_BAR_VALUE_CHANGED = 25;
	public const int MAW_EVENT_RADIO_GROUP_ITEM_SELECTED = 26;
	public const int MAW_EVENT_RADIO_BUTTON_STATE_CHANGED = 27;
	public const int MAW_EVENT_OPTIONS_MENU_CLOSED = 28;
	public const int MAW_EVENT_OPTIONS_MENU_ITEM_SELECTED = 29;
	public const int MAW_EVENT_MAP_ZOOM_LEVEL_CHANGED = 30;
	public const int MAW_EVENT_MAP_REGION_CHANGED = 31;
	public const int MAW_EVENT_MAP_PIN_CLICKED = 32;
	public const int MAW_EVENT_ITEM_WILL_SELECT = 33;
	public const int MAW_EVENT_SEGMENTED_LIST_ITEM_CLICKED = 34;
	public const int MAW_EVENT_SEGMENTED_LIST_ITEM_INSERT_BTN = 35;
	public const int MAW_EVENT_SEGMENTED_LIST_ITEM_DELETE_BTN = 36;
	public const String MAW_WIDGET_LEFT = "left";
	public const String MAW_WIDGET_TOP = "top";
	public const String MAW_WIDGET_WIDTH = "width";
	public const String MAW_WIDGET_HEIGHT = "height";
	public const String MAW_WIDGET_ALPHA = "alpha";
	public const String MAW_WIDGET_BACKGROUND_COLOR = "backgroundColor";
	public const String MAW_WIDGET_VISIBLE = "visible";
	public const String MAW_WIDGET_ENABLED = "enabled";
	public const String MAW_WIDGET_BACKGROUND_GRADIENT = "backgroundGradient";
	public const int MAW_RES_OK = 0;
	public const int MAW_RES_ERROR = -2;
	public const int MAW_RES_INVALID_PROPERTY_NAME = -2;
	public const int MAW_RES_INVALID_PROPERTY_VALUE = -3;
	public const int MAW_RES_INVALID_HANDLE = -4;
	public const int MAW_RES_INVALID_TYPE_NAME = -5;
	public const int MAW_RES_INVALID_INDEX = -6;
	public const int MAW_RES_INVALID_STRING_BUFFER_SIZE = -7;
	public const int MAW_RES_INVALID_SCREEN = -8;
	public const int MAW_RES_INVALID_LAYOUT = -9;
	public const int MAW_RES_REMOVED_ROOT = -10;
	public const int MAW_RES_FEATURE_NOT_AVAILABLE = -11;
	public const int MAW_RES_CANNOT_INSERT_DIALOG = -12;
	public const String MAW_SCREEN = "Screen";
	public const String MAW_TAB_SCREEN = "TabScreen";
	public const String MAW_STACK_SCREEN = "StackScreen";
	public const String MAW_BUTTON = "Button";
	public const String MAW_IMAGE = "Image";
	public const String MAW_IMAGE_BUTTON = "ImageButton";
	public const String MAW_LABEL = "Label";
	public const String MAW_EDIT_BOX = "EditBox";
	public const String MAW_LIST_VIEW = "ListView";
	public const String MAW_LIST_VIEW_ITEM = "ListViewItem";
	public const String MAW_CHECK_BOX = "CheckBox";
	public const String MAW_HORIZONTAL_LAYOUT = "HorizontalLayout";
	public const String MAW_VERTICAL_LAYOUT = "VerticalLayout";
	public const String MAW_RELATIVE_LAYOUT = "RelativeLayout";
	public const String MAW_SEARCH_BAR = "SearchBar";
	public const String MAW_NAV_BAR = "NavBar";
	public const String MAW_GL_VIEW = "GLView";
	public const String MAW_GL2_VIEW = "GL2View";
	public const String MAW_CAMERA_PREVIEW = "CameraPreview";
	public const String MAW_WEB_VIEW = "WebView";
	public const String MAW_PROGRESS_BAR = "ProgressBar";
	public const String MAW_ACTIVITY_INDICATOR = "ActivityIndicator";
	public const String MAW_SLIDER = "Slider";
	public const String MAW_DATE_PICKER = "DatePicker";
	public const String MAW_TIME_PICKER = "TimePicker";
	public const String MAW_NUMBER_PICKER = "NumberPicker";
	public const String MAW_VIDEO_VIEW = "VideoView";
	public const String MAW_TOGGLE_BUTTON = "ToggleButton";
	public const String MAW_MODAL_DIALOG = "ModalDialog";
	public const String MAW_PANORAMA_VIEW = "PanoramaView";
	public const String MAW_RADIO_BUTTON = "RadioButton";
	public const String MAW_RADIO_GROUP = "RadioGroup";
	public const String MAW_RATING_BAR = "RatingBar";
	public const String MAW_MAP = "Map";
	public const String MAW_MAP_PIN = "MapPin";
	public const String MAW_LIST_VIEW_SECTION = "ListViewSection";
	public const int MA_CAMERA_CONST_BACK_CAMERA = 0;
	public const int MA_CAMERA_CONST_FRONT_CAMERA = 1;
	public const int MA_CAMERA_RES_OK = 1;
	public const int MA_CAMERA_RES_FAILED = -2;
	public const int MA_CAMERA_RES_NOT_STARTED = -3;
	public const int MA_CAMERA_RES_PROPERTY_NOTSUPPORTED = -4;
	public const int MA_CAMERA_RES_INVALID_PROPERTY_VALUE = -5;
	public const int MA_CAMERA_RES_VALUE_NOTSUPPORTED = -6;
	public const int MA_CAMERA_RES_EVENTS_ALREADY_ENABLED = -7;
	public const int MA_CAMERA_RES_INVALID_PREVIEW_SIZE = -8;
	public const String MA_CAMERA_FLASH_ON = "on";
	public const String MA_CAMERA_FLASH_AUTO = "auto";
	public const String MA_CAMERA_FLASH_OFF = "off";
	public const String MA_CAMERA_FLASH_TORCH = "torch";
	public const String MA_CAMERA_FOCUS_AUTO = "auto";
	public const String MA_CAMERA_FOCUS_INFINITY = "infinity";
	public const String MA_CAMERA_FOCUS_MACRO = "macro";
	public const String MA_CAMERA_FOCUS_FIXED = "fixed";
	public const String MA_CAMERA_IMAGE_JPEG = "jpeg";
	public const String MA_CAMERA_IMAGE_RAW = "raw";
	public const String MA_CAMERA_FLASH_MODE = "flash-mode";
	public const String MA_CAMERA_FOCUS_MODE = "focus-mode";
	public const String MA_CAMERA_IMAGE_FORMAT = "image-format";
	public const String MA_CAMERA_ZOOM = "zoom";
	public const String MA_CAMERA_MAX_ZOOM = "max-zoom";
	public const String MA_CAMERA_ZOOM_SUPPORTED = "zoom-supported";
	public const String MA_CAMERA_FLASH_SUPPORTED = "flash-supported";
	public const int MA_CAMERA_PREVIEW_FRAME = 1;
	public const int MA_CAMERA_PREVIEW_AUTO_FOCUS = 2;
	public const int MA_TB_TYPE_ANY = 0;
	public const int MA_TB_TYPE_EMAILADDR = 1;
	public const int MA_TB_TYPE_NUMERIC = 2;
	public const int MA_TB_TYPE_PHONENUMBER = 3;
	public const int MA_TB_TYPE_URL = 4;
	public const int MA_TB_TYPE_DECIMAL = 5;
	public const int MA_TB_TYPE_SINGLE_LINE = 100;
	public const int MA_TB_TYPE_MASK = 0xFFFF;
	public const int MA_TB_RES_OK = 1;
	public const int MA_TB_RES_CANCEL = 2;
	public const int MA_TB_RES_TYPE_UNAVAILABLE = -3;
	public const int MA_TB_FLAG_PASSWORD = 0x10000;
	public const int MA_TB_FLAG_UNEDITABLE = 0x20000;
	public const int MA_TB_FLAG_SENSITIVE = 0x40000;
	public const int MA_TB_FLAG_NON_PREDICTIVE = 0x80000;
	public const int MA_TB_FLAG_INITIAL_CAPS_WORD = 0x100000;
	public const int MA_TB_FLAG_INITIAL_CAPS_SENTENCE = 0x200000;
	public const int NOTIFICATION_TYPE_APPLICATION_LAUNCHER = 1;
	public const int SCREEN_ORIENTATION_LANDSCAPE = 1;
	public const int SCREEN_ORIENTATION_PORTRAIT = 2;
	public const int SCREEN_ORIENTATION_DYNAMIC = 3;
	public const int MA_SCREEN_ORIENTATION_PORTRAIT = 0x000001;
	public const int MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN = 0x000002;
	public const int MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT = 0x000004;
	public const int MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT = 0x000008;
	public const int MA_SCREEN_ORIENTATION_RES_OK = 0;
	public const int MA_SCREEN_ORIENTATION_RES_NOT_SUPPORTED = -1;
	public const int MA_SCREEN_ORIENTATION_RES_INVALID_VALUE = -2;
	public const int MA_WAKE_LOCK_ON = 1;
	public const int MA_WAKE_LOCK_OFF = 2;
	public const int MA_GL_TEX_IMAGE_2D_OK = 0;
	public const int MA_GL_TEX_IMAGE_2D_INVALID_IMAGE = -2;
	public const int MA_GL_API_GL2 = 0;
	public const int MA_GL_API_GL1 = 1;
	public const int MA_GL_INIT_RES_OK = 0;
	public const int MA_GL_INIT_RES_UNAVAILABLE_API = -2;
	public const int MA_GL_INIT_RES_ERROR = -3;
	public const int SENSOR_TYPE_ACCELEROMETER = 1;
	public const int SENSOR_TYPE_MAGNETIC_FIELD = 2;
	public const int SENSOR_TYPE_ORIENTATION = 3;
	public const int SENSOR_TYPE_GYROSCOPE = 4;
	public const int SENSOR_TYPE_PROXIMITY = 5;
	public const int SENSOR_TYPE_COMPASS = 6;
	public const int SENSOR_RATE_FASTEST = 0;
	public const int SENSOR_RATE_GAME = -1;
	public const int SENSOR_RATE_NORMAL = -2;
	public const int SENSOR_RATE_UI = -3;
	public const int SENSOR_ERROR_NONE = 0;
	public const int SENSOR_ERROR_NOT_AVAILABLE = -1;
	public const int SENSOR_ERROR_INTERVAL_NOT_SET = -2;
	public const int SENSOR_ERROR_ALREADY_ENABLED = -3;
	public const int SENSOR_ERROR_NOT_ENABLED = -4;
	public const int SENSOR_ERROR_CANNOT_DISABLE = -5;
	public const int UIDEVICE_ORIENTATION_UNKNOWN = 0;
	public const int UIDEVICE_ORIENTATION_PORTRAIT = 1;
	public const int UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN = 2;
	public const int UIDEVICE_ORIENTATION_LANDSCAPE_LEFT = 3;
	public const int UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT = 4;
	public const int UIDEVICE_ORIENTATION_FACE_UP = 5;
	public const int UIDEVICE_ORIENTATION_FACE_DOWN = 6;
	public const int SENSOR_PROXIMITY_VALUE_FAR = 0;
	public const int SENSOR_PROXIMITY_VALUE_NEAR = 1;
	public const int MA_NFC_NOT_AVAILABLE = -1;
	public const int MA_NFC_NOT_ENABLED = -2;
	public const int MA_NFC_INVALID_TAG_TYPE = -2;
	public const int MA_NFC_TAG_CONNECTION_LOST = -3;
	public const int MA_NFC_TAG_NOT_CONNECTED = -4;
	public const int MA_NFC_FORMAT_FAILED = -5;
	public const int MA_NFC_TAG_IO_ERROR = -127;
	public const int MA_NFC_TAG_TYPE_NDEF = 1;
	public const int MA_NFC_TAG_TYPE_MIFARE_CL = 2;
	public const int MA_NFC_TAG_TYPE_MIFARE_UL = 3;
	public const int MA_NFC_TAG_TYPE_NFC_A = 4;
	public const int MA_NFC_TAG_TYPE_NFC_B = 5;
	public const int MA_NFC_TAG_TYPE_ISO_DEP = 6;
	public const int MA_NFC_TAG_TYPE_NDEF_FORMATTABLE = 128;
	public const int MA_NFC_NDEF_TNF_EMPTY = 0;
	public const int MA_NFC_NDEF_TNF_WELL_KNOWN = 1;
	public const int MA_NFC_NDEF_TNF_MIME_MEDIA = 2;
	public const int MA_NFC_NDEF_TNF_ABSOLUTE_URI = 3;
	public const int MA_NFC_NDEF_TNF_EXTERNAL_TYPE = 4;
	public const int MA_NFC_NDEF_TNF_UNKNOWN = 5;
	public const int MA_NFC_NDEF_TNF_UNCHANGED = 6;
	public const int MA_NFC_NDEF_TNF_RESERVED = 7;
	public const int MA_NFC_MIFARE_KEY_A = 1;
	public const int MA_NFC_MIFARE_KEY_B = 2;
	public const int MA_DB_TYPE_NULL = 1;
	public const int MA_DB_TYPE_INT = 2;
	public const int MA_DB_TYPE_INT64 = 3;
	public const int MA_DB_TYPE_DOUBLE = 4;
	public const int MA_DB_TYPE_BLOB = 5;
	public const int MA_DB_TYPE_DATA = 6;
	public const int MA_DB_TYPE_TEXT = 7;
	public const int MA_DB_OK = 0;
	public const int MA_DB_ERROR = -2;
	public const int MA_DB_NO_ROW = -3;
	public const int MA_DB_NULL = -4;
	public const int MA_AUDIO_DATA_STREAM = 1;
	public const int MA_AUDIO_ERR_OK = 0;
	public const int MA_AUDIO_ERR_INVALID_INSTANCE = -2;
	public const int MA_AUDIO_ERR_INVALID_DATA = -3;
	public const int MA_AUDIO_ERR_INVALID_FILENAME = -4;
	public const int MA_AUDIO_ERR_INVALID_FILE = -5;
	public const int MA_AUDIO_ERR_STREAMING_NOT_SUPPORTED = -6;
	public const int MA_AUDIO_ERR_VOLUME_OUT_OF_RANGE = -7;
	public const int MA_AUDIO_ERR_POSITION_OUT_OF_RANGE = -8;
	public const int MA_AUDIO_ERR_INVALID_SOUND_FORMAT = -9;
	public const int MA_AUDIO_ERR_OUT_OF_MEMORY = -10;
	public const int MA_AUDIO_ERR_ALREADY_PREPARED = -11;
	public const int MA_AUDIO_ERR_PREPARE_FAILED = -12;
	public const int MA_AUDIO_ERR_IS_PLAYING = -13;
	public const int MA_AUDIO_ERR_GENERIC = -14;
	public const int MA_ADS_RES_OK = 0;
	public const int MA_ADS_RES_INVALID_LAYOUT_HANDLE = -1;
	public const int MA_ADS_RES_INVALID_BANNER_HANDLE = -2;
	public const int MA_ADS_RES_INVALID_PROPERTY_NAME = -3;
	public const int MA_ADS_RES_INVALID_PROPERTY_VALUE = -4;
	public const int MA_ADS_RES_INVALID_STRING_BUFFER_SIZE = -5;
	public const int MA_ADS_RES_ERROR = -6;
	public const int MA_ADS_RES_UNSUPPORTED = -7;
	public const int MA_ADS_ERROR_INTERNAL = -1;
	public const int MA_ADS_ERROR_INVALID_REQUEST = -2;
	public const int MA_ADS_ERROR_NETWORK = -3;
	public const int MA_ADS_ERROR_NO_FILL = -4;
	public const int MA_ADS_ERROR_CONFIGURATION = -5;
	public const int MA_ADS_ERROR_NO_CONTENT = -6;
	public const int MA_ADS_ERROR_APPLICATION_INACTIVE = -7;
	public const String MA_ADS_HEIGHT = "height";
	public const String MA_ADS_WIDTH = "width";
	public const String MA_ADS_VISIBLE = "visible";
	public const String MA_ADS_ENABLED = "enabled";
	public const String MA_ADS_TEST_DEVICE = "testDevice";
	public const String MA_ADS_REQUEST_CONTENT = "requestContent";
	public const String MA_ADS_IS_READY = "isReady";
	public const String MA_ADS_COLOR_BG = "colorBg";
	public const String MA_ADS_COLOR_BG_TOP = "colorBgTop";
	public const String MA_ADS_COLOR_BORDER = "colorBorder";
	public const String MA_ADS_COLOR_LINK = "colorLink";
	public const String MA_ADS_COLOR_TEXT = "colorText";
	public const String MA_ADS_COLOR_URL = "colorUrl";
	public const int MA_ADS_SIZE_BANNER = 0;
	public const int MA_ADS_SIZE_RECT = 1;
	public const int MA_ADS_SIZE_IAB_BANNER = 2;
	public const int MA_ADS_SIZE_LEADERBOARD = 3;
	public const int MA_ADS_SIZE_WP7_XLARGE = 4;
	public const int MA_ADS_SIZE_WP7_XXLARGE = 5;
	public const int MA_ADS_EVENT_FAILED = 0;
	public const int MA_ADS_EVENT_LOADED = 1;
	public const int MA_ADS_EVENT_ON_DISMISS = 2;
	public const int MA_ADS_EVENT_ON_LEAVE_APPLICATION = 3;
	public const int MA_NOTIFICATION_RES_OK = 0;
	public const int MA_NOTIFICATION_RES_INVALID_HANDLE = -1;
	public const int MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME = -3;
	public const int MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE = -4;
	public const int MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE = -5;
	public const int MA_NOTIFICATION_RES_UNSUPPORTED = -6;
	public const int MA_NOTIFICATION_RES_ALREADY_SCHEDULED = -7;
	public const int MA_NOTIFICATION_RES_CANNOT_UNSCHEDULE = -8;
	public const int MA_NOTIFICATION_RES_ALREADY_REGISTERED = -9;
	public const int MA_NOTIFICATION_RES_NOT_REGISTERED = -10;
	public const int MA_NOTIFICATION_RES_REGISTRATION_IN_PROGRESS = -11;
	public const int MA_NOTIFICATION_RES_REGISTRATION_SERVICE_NOT_AVAILABLE = -12;
	public const int MA_NOTIFICATION_RES_REGISTRATION_ACCOUNT_MISSING = -13;
	public const int MA_NOTIFICATION_RES_REGISTRATION_AUTHENTICATION_FAILED = -14;
	public const int MA_NOTIFICATION_RES_REGISTRATION_TOO_MANY_REGISTRATIONS = -15;
	public const int MA_NOTIFICATION_RES_REGISTRATION_INVALID_SENDER = -16;
	public const int MA_NOTIFICATION_RES_REGISTRATION_PHONE_REGISTRATION_ERROR = -17;
	public const int MA_NOTIFICATION_RES_REGISTRATION_MESSAGE_BUF_TOO_SMALL = -18;
	public const int MA_NOTIFICATION_RES_REGISTRATION_NOT_CALLED = -19;
	public const int MA_NOTIFICATION_RES_ERROR = -20;
	public const int MA_NOTIFICATION_FLAG_INSISTENT = 4;
	public const int MA_NOTIFICATION_FLAG_NO_CLEAR = 32;
	public const int MA_NOTIFICATION_FLAG_HIGH_PRIORITY = 128;
	public const int MA_NOTIFICATION_FLAG_AUTO_CANCEL = 16;
	public const int MA_NOTIFICATION_DISPLAY_FLAG_DEFAULT = 0;
	public const int MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME = 1;
	public const int MA_NOTIFICATION_PUSH_TYPE_BADGE = 1;
	public const int MA_NOTIFICATION_PUSH_TYPE_SOUND = 2;
	public const int MA_NOTIFICATION_PUSH_TYPE_ALERT = 4;
	public const String MA_NOTIFICATION_LOCAL_BADGE_NUMBER = "badge_number";
	public const String MA_NOTIFICATION_LOCAL_FIRE_DATE = "fire_date";
	public const String MA_NOTIFICATION_LOCAL_CONTENT_BODY = "content_body";
	public const String MA_NOTIFICATION_LOCAL_ALERT_ACTION = "alert_action";
	public const String MA_NOTIFICATION_LOCAL_PLAY_SOUND = "play_sound";
	public const String MA_NOTIFICATION_LOCAL_SOUND_PATH = "soundPath";
	public const String MA_NOTIFICATION_LOCAL_CONTENT_TITLE = "contentTitle";
	public const String MA_NOTIFICATION_LOCAL_TICKER_TEXT = "tickerText";
	public const String MA_NOTIFICATION_LOCAL_FLAG = "flag";
	public const String MA_NOTIFICATION_LOCAL_DISPLAY_FLAG = "displayFlag";
	public const String MA_NOTIFICATION_LOCAL_VIBRATE = "vibrate";
	public const String MA_NOTIFICATION_LOCAL_VIBRATE_DURATION = "vibrateDuration";
	public const String MA_NOTIFICATION_LOCAL_FLASH_LIGHTS = "flashLights";
	public const String MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN = "flashLightsPattern";
	public const int MA_CAPTURE_RES_OK = 0;
	public const int MA_CAPTURE_RES_FILE_INVALID_NAME = -1;
	public const int MA_CAPTURE_RES_FILE_ALREADY_EXISTS = -2;
	public const int MA_CAPTURE_RES_INVALID_PROPERTY = -3;
	public const int MA_CAPTURE_RES_INVALID_PROPERTY_VALUE = -4;
	public const int MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE = -5;
	public const int MA_CAPTURE_RES_INVALID_ACTION = -6;
	public const int MA_CAPTURE_RES_INVALID_HANDLE = -7;
	public const int MA_CAPTURE_RES_UNSUPPORTED = -8;
	public const int MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE = -9;
	public const int MA_CAPTURE_RES_VIDEO_NOT_SUPPORTED = -10;
	public const int MA_CAPTURE_RES_PICTURE_NOT_SUPPORTED = -11;
	public const int MA_CAPTURE_RES_FLASH_NOT_SUPPORTED = -12;
	public const int MA_CAPTURE_RES_MAX_DURATION_NOT_SUPPORTED = -13;
	public const int MA_CAPTURE_RES_PROPERTY_VALUE_UNDEFINED = -14;
	public const String MA_CAPTURE_MAX_DURATION = "maxDuration";
	public const String MA_CAPTURE_VIDEO_QUALITY = "videoQuality";
	public const String MA_CAPTURE_CAMERA_ROLL = "cameraRoll";
	public const String MA_CAPTURE_FLASH = "flash";
	public const String MA_CAPTURE_CAMERA_CONTROLS = "cameraControls";
	public const int MA_CAPTURE_VIDEO_QUALITY_LOW = 0;
	public const int MA_CAPTURE_VIDEO_QUALITY_HIGH = 1;
	public const int MA_CAPTURE_VIDEO_QUALITY_MEDIUM = 2;
	public const int MA_CAPTURE_ACTION_RECORD_VIDEO = 0;
	public const int MA_CAPTURE_ACTION_STOP_RECORDING = 1;
	public const int MA_CAPTURE_ACTION_TAKE_PICTURE = 2;
	public const int MA_CAPTURE_EVENT_TYPE_IMAGE = 0;
	public const int MA_CAPTURE_EVENT_TYPE_VIDEO = 1;
	public const int MA_CAPTURE_EVENT_TYPE_CANCEL = 2;
	public const int MA_CAPTURE_FLASH_AUTO = 0;
	public const int MA_CAPTURE_FLASH_ON = 1;
	public const int MA_CAPTURE_FLASH_OFF = 2;
	public const int MA_PURCHASE_RES_OK = 0;
	public const int MA_PURCHASE_RES_UNAVAILABLE = -1;
	public const int MA_PURCHASE_RES_CONNECTION_ERROR = -2;
	public const int MA_PURCHASE_RES_OUT_OF_DATE = -3;
	public const int MA_PURCHASE_RES_CLIENT_INVALID = -4;
	public const int MA_PURCHASE_RES_NOT_ALLOWED = -5;
	public const int MA_PURCHASE_RES_DISABLED = -6;
	public const int MA_PURCHASE_RES_INVALID_HANDLE = -7;
	public const int MA_PURCHASE_RES_BUFFER_TOO_SMALL = -8;
	public const int MA_PURCHASE_RES_RECEIPT_NOT_AVAILABLE = -9;
	public const int MA_PURCHASE_RES_INVALID_FIELD_NAME = -10;
	public const int MA_PURCHASE_RES_MALFORMED_PUBLIC_KEY = -11;
	public const int MA_PURCHASE_ERROR_INVALID_HANDLE = -1;
	public const int MA_PURCHASE_ERROR_CONNECTION_FAILED = -2;
	public const int MA_PURCHASE_ERROR_CANNOT_PARSE_RECEIPT = -3;
	public const int MA_PURCHASE_ERROR_INVALID_PRODUCT = -4;
	public const int MA_PURCHASE_ERROR_INVALID_CLIENT = -5;
	public const int MA_PURCHASE_ERROR_CANCELLED = -6;
	public const int MA_PURCHASE_ERROR_NOT_ALLOWED = -7;
	public const int MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET = -8;
	public const int MA_PURCHASE_ERROR_UNKNOWN = -9;
	public const int MA_PURCHASE_ERROR_INVALID_QUANTITY = -10;
	public const int MA_PURCHASE_ERROR_NO_RECEIPT = -11;
	public const int MA_PURCHASE_EVENT_PRODUCT_CREATE = 0;
	public const int MA_PURCHASE_EVENT_REQUEST = 1;
	public const int MA_PURCHASE_EVENT_VERIFY_RECEIPT = 2;
	public const int MA_PURCHASE_EVENT_REFUNDED = 3;
	public const int MA_PURCHASE_EVENT_RESTORE = 4;
	public const int MA_PURCHASE_STATE_PRODUCT_VALID = 0;
	public const int MA_PURCHASE_STATE_PRODUCT_INVALID = 1;
	public const int MA_PURCHASE_STATE_DISABLED = 2;
	public const int MA_PURCHASE_STATE_DUPLICATE_HANDLE = 3;
	public const int MA_PURCHASE_STATE_FAILED = 0;
	public const int MA_PURCHASE_STATE_IN_PROGRESS = 1;
	public const int MA_PURCHASE_STATE_COMPLETED = 2;
	public const int MA_PURCHASE_STATE_RECEIPT_VALID = 4;
	public const int MA_PURCHASE_STATE_RECEIPT_INVALID = 5;
	public const int MA_PURCHASE_STATE_RECEIPT_ERROR = 6;
	public const int MA_PURCHASE_STATE_RESTORE_ERROR = -7;
	public const int MA_PURCHASE_STATE_PRODUCT_RESTORED = -8;
	public const int MA_PURCHASE_STATE_PRODUCT_REFUNDED = -9;
	public const String MA_PURCHASE_RECEIPT_QUANTITY = "quantity";
	public const String MA_PURCHASE_RECEIPT_PRODUCT_ID = "product_id";
	public const String MA_PURCHASE_RECEIPT_TRANSACTION_ID = "transaction_id";
	public const String MA_PURCHASE_RECEIPT_PURCHASE_DATE = "purchase_date";
	public const String MA_PURCHASE_RECEIPT_ORIGINAL_TRANSACTION_ID = "original_transaction_id";
	public const String MA_PURCHASE_RECEIPT_APP_ITEM_ID = "app_item_id";
	public const String MA_PURCHASE_RECEIPT_VERSION_EXTERNAL_IDENTIFIER = "version_external_identifier";
	public const String MA_PURCHASE_RECEIPT_BID = "bid";
	public const String MA_PURCHASE_RECEIPT_BVRS = "bvrs";
	public const String MA_PURCHASE_RECEIPT_PRICE = "price";
	public const String MA_PURCHASE_RECEIPT_TITLE = "title";
	public const String MA_PURCHASE_RECEIPT_DESCRIPTION = "description";
	public const int IOCTL_UNAVAILABLE = -1;
}

public class Syscalls {
	public delegate int Delegate_maCheckInterfaceVersion(int _hash);
	public Delegate_maCheckInterfaceVersion maCheckInterfaceVersion = null;
	public delegate void Delegate_maExit(int _result);
	public Delegate_maExit maExit = null;
	public delegate void Delegate_maPanic(int _result, int _message);
	public Delegate_maPanic maPanic = null;
	public delegate int Delegate_memset(int _dst, int _val, int _size);
	public Delegate_memset memset = null;
	public delegate int Delegate_memcpy(int _dst, int _src, int _size);
	public Delegate_memcpy memcpy = null;
	public delegate int Delegate_strcmp(int _str1, int _str2);
	public Delegate_strcmp strcmp = null;
	public delegate int Delegate_strcpy(int _dst, int _src);
	public Delegate_strcpy strcpy = null;
	public delegate double Delegate___adddf3(double _a, double _b);
	public Delegate___adddf3 __adddf3 = null;
	public delegate double Delegate___subdf3(double _a, double _b);
	public Delegate___subdf3 __subdf3 = null;
	public delegate double Delegate___muldf3(double _a, double _b);
	public Delegate___muldf3 __muldf3 = null;
	public delegate double Delegate___divdf3(double _a, double _b);
	public Delegate___divdf3 __divdf3 = null;
	public delegate double Delegate___negdf2(double _a);
	public Delegate___negdf2 __negdf2 = null;
	public delegate int Delegate___fixdfsi(double _a);
	public Delegate___fixdfsi __fixdfsi = null;
	public delegate int Delegate___fixunsdfsi(double _a);
	public Delegate___fixunsdfsi __fixunsdfsi = null;
	public delegate double Delegate___floatsidf(int _a);
	public Delegate___floatsidf __floatsidf = null;
	public delegate double Delegate___extendsfdf2(float _a);
	public Delegate___extendsfdf2 __extendsfdf2 = null;
	public delegate int Delegate_dcmp(double _a, double _b);
	public Delegate_dcmp dcmp = null;
	public delegate float Delegate___addsf3(float _a, float _b);
	public Delegate___addsf3 __addsf3 = null;
	public delegate float Delegate___subsf3(float _a, float _b);
	public Delegate___subsf3 __subsf3 = null;
	public delegate float Delegate___mulsf3(float _a, float _b);
	public Delegate___mulsf3 __mulsf3 = null;
	public delegate float Delegate___divsf3(float _a, float _b);
	public Delegate___divsf3 __divsf3 = null;
	public delegate float Delegate___negsf2(float _a);
	public Delegate___negsf2 __negsf2 = null;
	public delegate int Delegate___fixsfsi(float _a);
	public Delegate___fixsfsi __fixsfsi = null;
	public delegate int Delegate___fixunssfsi(float _a);
	public Delegate___fixunssfsi __fixunssfsi = null;
	public delegate float Delegate___floatsisf(int _a);
	public Delegate___floatsisf __floatsisf = null;
	public delegate float Delegate___truncdfsf2(double _a);
	public Delegate___truncdfsf2 __truncdfsf2 = null;
	public delegate int Delegate_fcmp(float _a, float _b);
	public Delegate_fcmp fcmp = null;
	public delegate double Delegate_sin(double _x);
	public Delegate_sin sin = null;
	public delegate double Delegate_cos(double _x);
	public Delegate_cos cos = null;
	public delegate double Delegate_tan(double _x);
	public Delegate_tan tan = null;
	public delegate double Delegate_sqrt(double _x);
	public Delegate_sqrt sqrt = null;
	public delegate int Delegate_maSetColor(int _rgb);
	public Delegate_maSetColor maSetColor = null;
	public delegate void Delegate_maSetClipRect(int _left, int _top, int _width, int _height);
	public Delegate_maSetClipRect maSetClipRect = null;
	public delegate void Delegate_maGetClipRect(int _out);
	public Delegate_maGetClipRect maGetClipRect = null;
	public delegate void Delegate_maPlot(int _posX, int _posY);
	public Delegate_maPlot maPlot = null;
	public delegate void Delegate_maLine(int _startX, int _startY, int _endX, int _endY);
	public Delegate_maLine maLine = null;
	public delegate void Delegate_maFillRect(int _left, int _top, int _width, int _height);
	public Delegate_maFillRect maFillRect = null;
	public delegate void Delegate_maFillTriangleStrip(int _points, int _count);
	public Delegate_maFillTriangleStrip maFillTriangleStrip = null;
	public delegate void Delegate_maFillTriangleFan(int _points, int _count);
	public Delegate_maFillTriangleFan maFillTriangleFan = null;
	public delegate int Delegate_maGetTextSize(int _str);
	public Delegate_maGetTextSize maGetTextSize = null;
	public delegate int Delegate_maGetTextSizeW(int _str);
	public Delegate_maGetTextSizeW maGetTextSizeW = null;
	public delegate void Delegate_maDrawText(int _left, int _top, int _str);
	public Delegate_maDrawText maDrawText = null;
	public delegate void Delegate_maDrawTextW(int _left, int _top, int _str);
	public Delegate_maDrawTextW maDrawTextW = null;
	public delegate void Delegate_maUpdateScreen();
	public Delegate_maUpdateScreen maUpdateScreen = null;
	public delegate void Delegate_maResetBacklight();
	public Delegate_maResetBacklight maResetBacklight = null;
	public delegate int Delegate_maGetScrSize();
	public Delegate_maGetScrSize maGetScrSize = null;
	public delegate void Delegate_maDrawImage(int _image, int _left, int _top);
	public Delegate_maDrawImage maDrawImage = null;
	public delegate void Delegate_maDrawRGB(int _dstPoint, int _src, int _srcRect, int _scanlength);
	public Delegate_maDrawRGB maDrawRGB = null;
	public delegate void Delegate_maDrawImageRegion(int _image, int _srcRect, int _dstPoint, int _transformMode);
	public Delegate_maDrawImageRegion maDrawImageRegion = null;
	public delegate int Delegate_maGetImageSize(int _image);
	public Delegate_maGetImageSize maGetImageSize = null;
	public delegate void Delegate_maGetImageData(int _image, int _dst, int _srcRect, int _scanlength);
	public Delegate_maGetImageData maGetImageData = null;
	public delegate int Delegate_maSetDrawTarget(int _image);
	public Delegate_maSetDrawTarget maSetDrawTarget = null;
	public delegate int Delegate_maFindLabel(int _name);
	public Delegate_maFindLabel maFindLabel = null;
	public delegate int Delegate_maCreateImageFromData(int _placeholder, int _data, int _offset, int _size);
	public Delegate_maCreateImageFromData maCreateImageFromData = null;
	public delegate int Delegate_maCreateImageRaw(int _placeholder, int _src, int _size, int _alpha);
	public Delegate_maCreateImageRaw maCreateImageRaw = null;
	public delegate int Delegate_maCreateDrawableImage(int _placeholder, int _width, int _height);
	public Delegate_maCreateDrawableImage maCreateDrawableImage = null;
	public delegate int Delegate_maCreateData(int _placeholder, int _size);
	public Delegate_maCreateData maCreateData = null;
	public delegate int Delegate_maCreatePlaceholder();
	public Delegate_maCreatePlaceholder maCreatePlaceholder = null;
	public delegate void Delegate_maDestroyPlaceholder(int _handle);
	public Delegate_maDestroyPlaceholder maDestroyPlaceholder = null;
	public delegate void Delegate_maDestroyObject(int _handle);
	public Delegate_maDestroyObject maDestroyObject = null;
	public delegate int Delegate_maGetDataSize(int _data);
	public Delegate_maGetDataSize maGetDataSize = null;
	public delegate void Delegate_maReadData(int _data, int _dst, int _offset, int _size);
	public Delegate_maReadData maReadData = null;
	public delegate void Delegate_maWriteData(int _data, int _src, int _offset, int _size);
	public Delegate_maWriteData maWriteData = null;
	public delegate void Delegate_maCopyData(int _params);
	public Delegate_maCopyData maCopyData = null;
	public delegate int Delegate_maOpenStore(int _name, int _flags);
	public Delegate_maOpenStore maOpenStore = null;
	public delegate int Delegate_maWriteStore(int _store, int _data);
	public Delegate_maWriteStore maWriteStore = null;
	public delegate int Delegate_maReadStore(int _store, int _placeholder);
	public Delegate_maReadStore maReadStore = null;
	public delegate void Delegate_maCloseStore(int _store, int __delete);
	public Delegate_maCloseStore maCloseStore = null;
	public delegate int Delegate_maConnect(int _url);
	public Delegate_maConnect maConnect = null;
	public delegate void Delegate_maConnClose(int _conn);
	public Delegate_maConnClose maConnClose = null;
	public delegate void Delegate_maConnRead(int _conn, int _dst, int _size);
	public Delegate_maConnRead maConnRead = null;
	public delegate void Delegate_maConnWrite(int _conn, int _src, int _size);
	public Delegate_maConnWrite maConnWrite = null;
	public delegate void Delegate_maConnReadToData(int _conn, int _data, int _offset, int _size);
	public Delegate_maConnReadToData maConnReadToData = null;
	public delegate void Delegate_maConnWriteFromData(int _conn, int _data, int _offset, int _size);
	public Delegate_maConnWriteFromData maConnWriteFromData = null;
	public delegate int Delegate_maConnGetAddr(int _conn, int _addr);
	public Delegate_maConnGetAddr maConnGetAddr = null;
	public delegate int Delegate_maHttpCreate(int _url, int _method);
	public Delegate_maHttpCreate maHttpCreate = null;
	public delegate void Delegate_maHttpSetRequestHeader(int _conn, int _key, int _value);
	public Delegate_maHttpSetRequestHeader maHttpSetRequestHeader = null;
	public delegate int Delegate_maHttpGetResponseHeader(int _conn, int _key, int _buffer, int _bufSize);
	public Delegate_maHttpGetResponseHeader maHttpGetResponseHeader = null;
	public delegate void Delegate_maHttpFinish(int _conn);
	public Delegate_maHttpFinish maHttpFinish = null;
	public delegate int Delegate_maLoadResources(int _data);
	public Delegate_maLoadResources maLoadResources = null;
	public delegate int Delegate_maLoadResource(int _handle, int _placeholder, int _flag);
	public Delegate_maLoadResource maLoadResource = null;
	public delegate int Delegate_maCountResources();
	public Delegate_maCountResources maCountResources = null;
	public delegate void Delegate_maLoadProgram(int _data, int _reload);
	public Delegate_maLoadProgram maLoadProgram = null;
	public delegate int Delegate_maGetKeys();
	public Delegate_maGetKeys maGetKeys = null;
	public delegate int Delegate_maGetEvent(int _event);
	public Delegate_maGetEvent maGetEvent = null;
	public delegate void Delegate_maWait(int _timeout);
	public Delegate_maWait maWait = null;
	public delegate int Delegate_maTime();
	public Delegate_maTime maTime = null;
	public delegate int Delegate_maLocalTime();
	public Delegate_maLocalTime maLocalTime = null;
	public delegate int Delegate_maGetMilliSecondCount();
	public Delegate_maGetMilliSecondCount maGetMilliSecondCount = null;
	public delegate int Delegate_maFreeObjectMemory();
	public Delegate_maFreeObjectMemory maFreeObjectMemory = null;
	public delegate int Delegate_maTotalObjectMemory();
	public Delegate_maTotalObjectMemory maTotalObjectMemory = null;
	public delegate int Delegate_maVibrate(int _ms);
	public Delegate_maVibrate maVibrate = null;
	public delegate int Delegate_maSoundPlay(int _sound_res, int _offset, int _size);
	public Delegate_maSoundPlay maSoundPlay = null;
	public delegate void Delegate_maSoundStop();
	public Delegate_maSoundStop maSoundStop = null;
	public delegate int Delegate_maSoundIsPlaying();
	public Delegate_maSoundIsPlaying maSoundIsPlaying = null;
	public delegate int Delegate_maSoundGetVolume();
	public Delegate_maSoundGetVolume maSoundGetVolume = null;
	public delegate void Delegate_maSoundSetVolume(int _vol);
	public Delegate_maSoundSetVolume maSoundSetVolume = null;
	public delegate long Delegate_maExtensionFunctionInvoke(int _function, int _a, int _b, int _c);
	public Delegate_maExtensionFunctionInvoke maExtensionFunctionInvoke = null;
	public delegate long Delegate_maIOCtl(int _function, int _a, int _b, int _c);
	public Delegate_maIOCtl maIOCtl = null;
}

public class SyscallInvoker {

	private CoreInterpreted mCore;
	private Syscalls mSyscalls;

	public SyscallInvoker(CoreInterpreted core, Syscalls syscalls) {
		mCore = core;
		mSyscalls = syscalls;
	}

	public void InvokeSyscall(int id) {
		switch(id) {
			case 1:
			mCore.SetReturnValue(mSyscalls.maCheckInterfaceVersion(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maCheckInterfaceVersion("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 2:
			mSyscalls.maExit(mCore.GetRegisterValue(CoreInterpreted.Reg.I0));
#if SYSCALL_LOG
			Util.Log("maExit("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				")\n");
#endif
			break;
			case 3:
			mSyscalls.maPanic(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1));
#if SYSCALL_LOG
			Util.Log("maPanic("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				")\n");
#endif
			break;
			case 4:
			mCore.SetReturnValue(mSyscalls.memset(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2)));
#if SYSCALL_LOG
			Util.Log("memset("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 5:
			mCore.SetReturnValue(mSyscalls.memcpy(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2)));
#if SYSCALL_LOG
			Util.Log("memcpy("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 6:
			mCore.SetReturnValue(mSyscalls.strcmp(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)));
#if SYSCALL_LOG
			Util.Log("strcmp("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 7:
			mCore.SetReturnValue(mSyscalls.strcpy(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)));
#if SYSCALL_LOG
			Util.Log("strcpy("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 8:
			mCore.SetReturnValue(mSyscalls.__adddf3(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)), MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))));
#if SYSCALL_LOG
			Util.Log("__adddf3("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				","+MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 9:
			mCore.SetReturnValue(mSyscalls.__subdf3(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)), MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))));
#if SYSCALL_LOG
			Util.Log("__subdf3("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				","+MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 10:
			mCore.SetReturnValue(mSyscalls.__muldf3(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)), MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))));
#if SYSCALL_LOG
			Util.Log("__muldf3("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				","+MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 11:
			mCore.SetReturnValue(mSyscalls.__divdf3(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)), MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))));
#if SYSCALL_LOG
			Util.Log("__divdf3("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				","+MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 12:
			mCore.SetReturnValue(mSyscalls.__negdf2(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("__negdf2("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 13:
			mCore.SetReturnValue(mSyscalls.__fixdfsi(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("__fixdfsi("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 14:
			mCore.SetReturnValue(mSyscalls.__fixunsdfsi(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("__fixunsdfsi("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 15:
			mCore.SetReturnValue(mSyscalls.__floatsidf(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("__floatsidf("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 16:
			mCore.SetReturnValue(mSyscalls.__extendsfdf2(MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))));
#if SYSCALL_LOG
			Util.Log("__extendsfdf2("+
				MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 17:
			mCore.SetReturnValue(mSyscalls.dcmp(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)), MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))));
#if SYSCALL_LOG
			Util.Log("dcmp("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				","+MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 18:
			mCore.SetReturnValue(mSyscalls.__addsf3(MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)), MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("__addsf3("+
				MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 19:
			mCore.SetReturnValue(mSyscalls.__subsf3(MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)), MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("__subsf3("+
				MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 20:
			mCore.SetReturnValue(mSyscalls.__mulsf3(MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)), MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("__mulsf3("+
				MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 21:
			mCore.SetReturnValue(mSyscalls.__divsf3(MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)), MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("__divsf3("+
				MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 22:
			mCore.SetReturnValue(mSyscalls.__negsf2(MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))));
#if SYSCALL_LOG
			Util.Log("__negsf2("+
				MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 23:
			mCore.SetReturnValue(mSyscalls.__fixsfsi(MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))));
#if SYSCALL_LOG
			Util.Log("__fixsfsi("+
				MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 24:
			mCore.SetReturnValue(mSyscalls.__fixunssfsi(MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))));
#if SYSCALL_LOG
			Util.Log("__fixunssfsi("+
				MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 25:
			mCore.SetReturnValue(mSyscalls.__floatsisf(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("__floatsisf("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 26:
			mCore.SetReturnValue(mSyscalls.__truncdfsf2(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("__truncdfsf2("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 27:
			mCore.SetReturnValue(mSyscalls.fcmp(MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)), MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("fcmp("+
				MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 28:
			mCore.SetReturnValue(mSyscalls.sin(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("sin("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 29:
			mCore.SetReturnValue(mSyscalls.cos(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("cos("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 30:
			mCore.SetReturnValue(mSyscalls.tan(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("tan("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 31:
			mCore.SetReturnValue(mSyscalls.sqrt(MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))));
#if SYSCALL_LOG
			Util.Log("sqrt("+
				MoSync.Util.ConvertToDouble(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1))+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 32:
			mCore.SetReturnValue(mSyscalls.maSetColor(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maSetColor("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 33:
			mSyscalls.maSetClipRect(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maSetClipRect("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 34:
			mSyscalls.maGetClipRect(mCore.GetRegisterValue(CoreInterpreted.Reg.I0));
#if SYSCALL_LOG
			Util.Log("maGetClipRect("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				")\n");
#endif
			break;
			case 35:
			mSyscalls.maPlot(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1));
#if SYSCALL_LOG
			Util.Log("maPlot("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				")\n");
#endif
			break;
			case 36:
			mSyscalls.maLine(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maLine("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 37:
			mSyscalls.maFillRect(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maFillRect("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 38:
			mSyscalls.maFillTriangleStrip(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1));
#if SYSCALL_LOG
			Util.Log("maFillTriangleStrip("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				")\n");
#endif
			break;
			case 39:
			mSyscalls.maFillTriangleFan(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1));
#if SYSCALL_LOG
			Util.Log("maFillTriangleFan("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				")\n");
#endif
			break;
			case 40:
			mCore.SetReturnValue(mSyscalls.maGetTextSize(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maGetTextSize("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 41:
			mCore.SetReturnValue(mSyscalls.maGetTextSizeW(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maGetTextSizeW("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 42:
			mSyscalls.maDrawText(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2));
#if SYSCALL_LOG
			Util.Log("maDrawText("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				")\n");
#endif
			break;
			case 43:
			mSyscalls.maDrawTextW(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2));
#if SYSCALL_LOG
			Util.Log("maDrawTextW("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				")\n");
#endif
			break;
			case 44:
			mSyscalls.maUpdateScreen();
#if SYSCALL_LOG
			Util.Log("maUpdateScreen("+
				")\n");
#endif
			break;
			case 45:
			mSyscalls.maResetBacklight();
#if SYSCALL_LOG
			Util.Log("maResetBacklight("+
				")\n");
#endif
			break;
			case 46:
			mCore.SetReturnValue(mSyscalls.maGetScrSize());
#if SYSCALL_LOG
			Util.Log("maGetScrSize("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 47:
			mSyscalls.maDrawImage(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2));
#if SYSCALL_LOG
			Util.Log("maDrawImage("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				")\n");
#endif
			break;
			case 48:
			mSyscalls.maDrawRGB(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maDrawRGB("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 49:
			mSyscalls.maDrawImageRegion(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maDrawImageRegion("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 50:
			mCore.SetReturnValue(mSyscalls.maGetImageSize(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maGetImageSize("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 51:
			mSyscalls.maGetImageData(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maGetImageData("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 52:
			mCore.SetReturnValue(mSyscalls.maSetDrawTarget(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maSetDrawTarget("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 53:
			mCore.SetReturnValue(mSyscalls.maFindLabel(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maFindLabel("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 54:
			mCore.SetReturnValue(mSyscalls.maCreateImageFromData(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3)));
#if SYSCALL_LOG
			Util.Log("maCreateImageFromData("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 55:
			mCore.SetReturnValue(mSyscalls.maCreateImageRaw(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3)));
#if SYSCALL_LOG
			Util.Log("maCreateImageRaw("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 56:
			mCore.SetReturnValue(mSyscalls.maCreateDrawableImage(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2)));
#if SYSCALL_LOG
			Util.Log("maCreateDrawableImage("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 57:
			mCore.SetReturnValue(mSyscalls.maCreateData(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)));
#if SYSCALL_LOG
			Util.Log("maCreateData("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 58:
			mCore.SetReturnValue(mSyscalls.maCreatePlaceholder());
#if SYSCALL_LOG
			Util.Log("maCreatePlaceholder("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 59:
			mSyscalls.maDestroyPlaceholder(mCore.GetRegisterValue(CoreInterpreted.Reg.I0));
#if SYSCALL_LOG
			Util.Log("maDestroyPlaceholder("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				")\n");
#endif
			break;
			case 60:
			mSyscalls.maDestroyObject(mCore.GetRegisterValue(CoreInterpreted.Reg.I0));
#if SYSCALL_LOG
			Util.Log("maDestroyObject("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				")\n");
#endif
			break;
			case 61:
			mCore.SetReturnValue(mSyscalls.maGetDataSize(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maGetDataSize("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 62:
			mSyscalls.maReadData(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maReadData("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 63:
			mSyscalls.maWriteData(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maWriteData("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 64:
			mSyscalls.maCopyData(mCore.GetRegisterValue(CoreInterpreted.Reg.I0));
#if SYSCALL_LOG
			Util.Log("maCopyData("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				")\n");
#endif
			break;
			case 65:
			mCore.SetReturnValue(mSyscalls.maOpenStore(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)));
#if SYSCALL_LOG
			Util.Log("maOpenStore("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 66:
			mCore.SetReturnValue(mSyscalls.maWriteStore(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)));
#if SYSCALL_LOG
			Util.Log("maWriteStore("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 67:
			mCore.SetReturnValue(mSyscalls.maReadStore(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)));
#if SYSCALL_LOG
			Util.Log("maReadStore("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 68:
			mSyscalls.maCloseStore(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1));
#if SYSCALL_LOG
			Util.Log("maCloseStore("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				")\n");
#endif
			break;
			case 69:
			mCore.SetReturnValue(mSyscalls.maConnect(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maConnect("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 70:
			mSyscalls.maConnClose(mCore.GetRegisterValue(CoreInterpreted.Reg.I0));
#if SYSCALL_LOG
			Util.Log("maConnClose("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				")\n");
#endif
			break;
			case 71:
			mSyscalls.maConnRead(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2));
#if SYSCALL_LOG
			Util.Log("maConnRead("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				")\n");
#endif
			break;
			case 72:
			mSyscalls.maConnWrite(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2));
#if SYSCALL_LOG
			Util.Log("maConnWrite("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				")\n");
#endif
			break;
			case 73:
			mSyscalls.maConnReadToData(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maConnReadToData("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 74:
			mSyscalls.maConnWriteFromData(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3));
#if SYSCALL_LOG
			Util.Log("maConnWriteFromData("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				")\n");
#endif
			break;
			case 75:
			mCore.SetReturnValue(mSyscalls.maConnGetAddr(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)));
#if SYSCALL_LOG
			Util.Log("maConnGetAddr("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 76:
			mCore.SetReturnValue(mSyscalls.maHttpCreate(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1)));
#if SYSCALL_LOG
			Util.Log("maHttpCreate("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 77:
			mSyscalls.maHttpSetRequestHeader(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2));
#if SYSCALL_LOG
			Util.Log("maHttpSetRequestHeader("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				")\n");
#endif
			break;
			case 78:
			mCore.SetReturnValue(mSyscalls.maHttpGetResponseHeader(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3)));
#if SYSCALL_LOG
			Util.Log("maHttpGetResponseHeader("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 79:
			mSyscalls.maHttpFinish(mCore.GetRegisterValue(CoreInterpreted.Reg.I0));
#if SYSCALL_LOG
			Util.Log("maHttpFinish("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				")\n");
#endif
			break;
			case 80:
			mCore.SetReturnValue(mSyscalls.maLoadResources(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maLoadResources("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 81:
			mCore.SetReturnValue(mSyscalls.maLoadResource(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2)));
#if SYSCALL_LOG
			Util.Log("maLoadResource("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 82:
			mCore.SetReturnValue(mSyscalls.maCountResources());
#if SYSCALL_LOG
			Util.Log("maCountResources("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 83:
			mSyscalls.maLoadProgram(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1));
#if SYSCALL_LOG
			Util.Log("maLoadProgram("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				")\n");
#endif
			break;
			case 84:
			mCore.SetReturnValue(mSyscalls.maGetKeys());
#if SYSCALL_LOG
			Util.Log("maGetKeys("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 85:
			mCore.SetReturnValue(mSyscalls.maGetEvent(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maGetEvent("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 86:
			mSyscalls.maWait(mCore.GetRegisterValue(CoreInterpreted.Reg.I0));
#if SYSCALL_LOG
			Util.Log("maWait("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				")\n");
#endif
			break;
			case 87:
			mCore.SetReturnValue(mSyscalls.maTime());
#if SYSCALL_LOG
			Util.Log("maTime("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 88:
			mCore.SetReturnValue(mSyscalls.maLocalTime());
#if SYSCALL_LOG
			Util.Log("maLocalTime("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 89:
			mCore.SetReturnValue(mSyscalls.maGetMilliSecondCount());
#if SYSCALL_LOG
			Util.Log("maGetMilliSecondCount("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 90:
			mCore.SetReturnValue(mSyscalls.maFreeObjectMemory());
#if SYSCALL_LOG
			Util.Log("maFreeObjectMemory("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 91:
			mCore.SetReturnValue(mSyscalls.maTotalObjectMemory());
#if SYSCALL_LOG
			Util.Log("maTotalObjectMemory("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 92:
			mCore.SetReturnValue(mSyscalls.maVibrate(mCore.GetRegisterValue(CoreInterpreted.Reg.I0)));
#if SYSCALL_LOG
			Util.Log("maVibrate("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 93:
			mCore.SetReturnValue(mSyscalls.maSoundPlay(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2)));
#if SYSCALL_LOG
			Util.Log("maSoundPlay("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 94:
			mSyscalls.maSoundStop();
#if SYSCALL_LOG
			Util.Log("maSoundStop("+
				")\n");
#endif
			break;
			case 95:
			mCore.SetReturnValue(mSyscalls.maSoundIsPlaying());
#if SYSCALL_LOG
			Util.Log("maSoundIsPlaying("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 96:
			mCore.SetReturnValue(mSyscalls.maSoundGetVolume());
#if SYSCALL_LOG
			Util.Log("maSoundGetVolume("+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 97:
			mSyscalls.maSoundSetVolume(mCore.GetRegisterValue(CoreInterpreted.Reg.I0));
#if SYSCALL_LOG
			Util.Log("maSoundSetVolume("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				")\n");
#endif
			break;
			case 98:
			mCore.SetReturnValue(mSyscalls.maExtensionFunctionInvoke(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3)));
#if SYSCALL_LOG
			Util.Log("maExtensionFunctionInvoke("+
				mCore.GetRegisterValue(CoreInterpreted.Reg.I0)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I1)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I2)+
				","+mCore.GetRegisterValue(CoreInterpreted.Reg.I3)+
				"): "+mCore.GetRegisterValue(CoreInterpreted.Reg.R14)+"\n");
#endif
			break;
			case 99:
			mCore.SetReturnValue(mSyscalls.maIOCtl(mCore.GetRegisterValue(CoreInterpreted.Reg.I0), mCore.GetRegisterValue(CoreInterpreted.Reg.I1), mCore.GetRegisterValue(CoreInterpreted.Reg.I2), mCore.GetRegisterValue(CoreInterpreted.Reg.I3)));
			break;
		}
	}
}
public class CoreNativeSyscallInvoker
{
	protected CoreNative mCore;
	protected Syscalls mSyscalls;
	public CoreNativeSyscallInvoker(CoreNative core, Syscalls syscalls)
	{
		mCore = core;
		mSyscalls = syscalls;
	}

	public int maCheckInterfaceVersion(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maCheckInterfaceVersion(i0));
	}
	public void maExit(int i0)
	{
		mSyscalls.maExit(i0);
	}
	public void maPanic(int i0, int i1)
	{
		mSyscalls.maPanic(i0, i1);
	}
	public int memset(int i0, int i1, int i2)
	{
		return mCore.SetReturnValue(mSyscalls.memset(i0, i1, i2));
	}
	public int memcpy(int i0, int i1, int i2)
	{
		return mCore.SetReturnValue(mSyscalls.memcpy(i0, i1, i2));
	}
	public int strcmp(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.strcmp(i0, i1));
	}
	public int strcpy(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.strcpy(i0, i1));
	}
	public int __adddf3(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.__adddf3(MoSync.Util.ConvertToDouble(i0, i1), MoSync.Util.ConvertToDouble(i2, i3)));
	}
	public int __subdf3(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.__subdf3(MoSync.Util.ConvertToDouble(i0, i1), MoSync.Util.ConvertToDouble(i2, i3)));
	}
	public int __muldf3(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.__muldf3(MoSync.Util.ConvertToDouble(i0, i1), MoSync.Util.ConvertToDouble(i2, i3)));
	}
	public int __divdf3(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.__divdf3(MoSync.Util.ConvertToDouble(i0, i1), MoSync.Util.ConvertToDouble(i2, i3)));
	}
	public int __negdf2(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.__negdf2(MoSync.Util.ConvertToDouble(i0, i1)));
	}
	public int __fixdfsi(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.__fixdfsi(MoSync.Util.ConvertToDouble(i0, i1)));
	}
	public int __fixunsdfsi(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.__fixunsdfsi(MoSync.Util.ConvertToDouble(i0, i1)));
	}
	public int __floatsidf(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.__floatsidf(i0));
	}
	public int __extendsfdf2(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.__extendsfdf2(MoSync.Util.ConvertToFloat(i0)));
	}
	public int dcmp(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.dcmp(MoSync.Util.ConvertToDouble(i0, i1), MoSync.Util.ConvertToDouble(i2, i3)));
	}
	public int __addsf3(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.__addsf3(MoSync.Util.ConvertToFloat(i0), MoSync.Util.ConvertToFloat(i1)));
	}
	public int __subsf3(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.__subsf3(MoSync.Util.ConvertToFloat(i0), MoSync.Util.ConvertToFloat(i1)));
	}
	public int __mulsf3(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.__mulsf3(MoSync.Util.ConvertToFloat(i0), MoSync.Util.ConvertToFloat(i1)));
	}
	public int __divsf3(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.__divsf3(MoSync.Util.ConvertToFloat(i0), MoSync.Util.ConvertToFloat(i1)));
	}
	public int __negsf2(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.__negsf2(MoSync.Util.ConvertToFloat(i0)));
	}
	public int __fixsfsi(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.__fixsfsi(MoSync.Util.ConvertToFloat(i0)));
	}
	public int __fixunssfsi(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.__fixunssfsi(MoSync.Util.ConvertToFloat(i0)));
	}
	public int __floatsisf(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.__floatsisf(i0));
	}
	public int __truncdfsf2(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.__truncdfsf2(MoSync.Util.ConvertToDouble(i0, i1)));
	}
	public int fcmp(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.fcmp(MoSync.Util.ConvertToFloat(i0), MoSync.Util.ConvertToFloat(i1)));
	}
	public int sin(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.sin(MoSync.Util.ConvertToDouble(i0, i1)));
	}
	public int cos(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.cos(MoSync.Util.ConvertToDouble(i0, i1)));
	}
	public int tan(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.tan(MoSync.Util.ConvertToDouble(i0, i1)));
	}
	public int sqrt(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.sqrt(MoSync.Util.ConvertToDouble(i0, i1)));
	}
	public int maSetColor(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maSetColor(i0));
	}
	public void maSetClipRect(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maSetClipRect(i0, i1, i2, i3);
	}
	public void maGetClipRect(int i0)
	{
		mSyscalls.maGetClipRect(i0);
	}
	public void maPlot(int i0, int i1)
	{
		mSyscalls.maPlot(i0, i1);
	}
	public void maLine(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maLine(i0, i1, i2, i3);
	}
	public void maFillRect(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maFillRect(i0, i1, i2, i3);
	}
	public void maFillTriangleStrip(int i0, int i1)
	{
		mSyscalls.maFillTriangleStrip(i0, i1);
	}
	public void maFillTriangleFan(int i0, int i1)
	{
		mSyscalls.maFillTriangleFan(i0, i1);
	}
	public int maGetTextSize(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maGetTextSize(i0));
	}
	public int maGetTextSizeW(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maGetTextSizeW(i0));
	}
	public void maDrawText(int i0, int i1, int i2)
	{
		mSyscalls.maDrawText(i0, i1, i2);
	}
	public void maDrawTextW(int i0, int i1, int i2)
	{
		mSyscalls.maDrawTextW(i0, i1, i2);
	}
	public void maUpdateScreen()
	{
		mSyscalls.maUpdateScreen();
	}
	public void maResetBacklight()
	{
		mSyscalls.maResetBacklight();
	}
	public int maGetScrSize()
	{
		return mCore.SetReturnValue(mSyscalls.maGetScrSize());
	}
	public void maDrawImage(int i0, int i1, int i2)
	{
		mSyscalls.maDrawImage(i0, i1, i2);
	}
	public void maDrawRGB(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maDrawRGB(i0, i1, i2, i3);
	}
	public void maDrawImageRegion(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maDrawImageRegion(i0, i1, i2, i3);
	}
	public int maGetImageSize(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maGetImageSize(i0));
	}
	public void maGetImageData(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maGetImageData(i0, i1, i2, i3);
	}
	public int maSetDrawTarget(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maSetDrawTarget(i0));
	}
	public int maFindLabel(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maFindLabel(i0));
	}
	public int maCreateImageFromData(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.maCreateImageFromData(i0, i1, i2, i3));
	}
	public int maCreateImageRaw(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.maCreateImageRaw(i0, i1, i2, i3));
	}
	public int maCreateDrawableImage(int i0, int i1, int i2)
	{
		return mCore.SetReturnValue(mSyscalls.maCreateDrawableImage(i0, i1, i2));
	}
	public int maCreateData(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.maCreateData(i0, i1));
	}
	public int maCreatePlaceholder()
	{
		return mCore.SetReturnValue(mSyscalls.maCreatePlaceholder());
	}
	public void maDestroyPlaceholder(int i0)
	{
		mSyscalls.maDestroyPlaceholder(i0);
	}
	public void maDestroyObject(int i0)
	{
		mSyscalls.maDestroyObject(i0);
	}
	public int maGetDataSize(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maGetDataSize(i0));
	}
	public void maReadData(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maReadData(i0, i1, i2, i3);
	}
	public void maWriteData(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maWriteData(i0, i1, i2, i3);
	}
	public void maCopyData(int i0)
	{
		mSyscalls.maCopyData(i0);
	}
	public int maOpenStore(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.maOpenStore(i0, i1));
	}
	public int maWriteStore(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.maWriteStore(i0, i1));
	}
	public int maReadStore(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.maReadStore(i0, i1));
	}
	public void maCloseStore(int i0, int i1)
	{
		mSyscalls.maCloseStore(i0, i1);
	}
	public int maConnect(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maConnect(i0));
	}
	public void maConnClose(int i0)
	{
		mSyscalls.maConnClose(i0);
	}
	public void maConnRead(int i0, int i1, int i2)
	{
		mSyscalls.maConnRead(i0, i1, i2);
	}
	public void maConnWrite(int i0, int i1, int i2)
	{
		mSyscalls.maConnWrite(i0, i1, i2);
	}
	public void maConnReadToData(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maConnReadToData(i0, i1, i2, i3);
	}
	public void maConnWriteFromData(int i0, int i1, int i2, int i3)
	{
		mSyscalls.maConnWriteFromData(i0, i1, i2, i3);
	}
	public int maConnGetAddr(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.maConnGetAddr(i0, i1));
	}
	public int maHttpCreate(int i0, int i1)
	{
		return mCore.SetReturnValue(mSyscalls.maHttpCreate(i0, i1));
	}
	public void maHttpSetRequestHeader(int i0, int i1, int i2)
	{
		mSyscalls.maHttpSetRequestHeader(i0, i1, i2);
	}
	public int maHttpGetResponseHeader(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.maHttpGetResponseHeader(i0, i1, i2, i3));
	}
	public void maHttpFinish(int i0)
	{
		mSyscalls.maHttpFinish(i0);
	}
	public int maLoadResources(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maLoadResources(i0));
	}
	public int maLoadResource(int i0, int i1, int i2)
	{
		return mCore.SetReturnValue(mSyscalls.maLoadResource(i0, i1, i2));
	}
	public int maCountResources()
	{
		return mCore.SetReturnValue(mSyscalls.maCountResources());
	}
	public void maLoadProgram(int i0, int i1)
	{
		mSyscalls.maLoadProgram(i0, i1);
	}
	public int maGetKeys()
	{
		return mCore.SetReturnValue(mSyscalls.maGetKeys());
	}
	public int maGetEvent(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maGetEvent(i0));
	}
	public void maWait(int i0)
	{
		mSyscalls.maWait(i0);
	}
	public int maTime()
	{
		return mCore.SetReturnValue(mSyscalls.maTime());
	}
	public int maLocalTime()
	{
		return mCore.SetReturnValue(mSyscalls.maLocalTime());
	}
	public int maGetMilliSecondCount()
	{
		return mCore.SetReturnValue(mSyscalls.maGetMilliSecondCount());
	}
	public int maFreeObjectMemory()
	{
		return mCore.SetReturnValue(mSyscalls.maFreeObjectMemory());
	}
	public int maTotalObjectMemory()
	{
		return mCore.SetReturnValue(mSyscalls.maTotalObjectMemory());
	}
	public int maVibrate(int i0)
	{
		return mCore.SetReturnValue(mSyscalls.maVibrate(i0));
	}
	public int maSoundPlay(int i0, int i1, int i2)
	{
		return mCore.SetReturnValue(mSyscalls.maSoundPlay(i0, i1, i2));
	}
	public void maSoundStop()
	{
		mSyscalls.maSoundStop();
	}
	public int maSoundIsPlaying()
	{
		return mCore.SetReturnValue(mSyscalls.maSoundIsPlaying());
	}
	public int maSoundGetVolume()
	{
		return mCore.SetReturnValue(mSyscalls.maSoundGetVolume());
	}
	public void maSoundSetVolume(int i0)
	{
		mSyscalls.maSoundSetVolume(i0);
	}
	public int maExtensionFunctionInvoke(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.maExtensionFunctionInvoke(i0, i1, i2, i3));
	}
	public int maIOCtl(int i0, int i1, int i2, int i3)
	{
		return mCore.SetReturnValue(mSyscalls.maIOCtl(i0, i1, i2, i3));
	}
}
public class Ioctls {
	public delegate long Delegate_maExtensionModuleLoad(int _id, int _hash);
	public Delegate_maExtensionModuleLoad maExtensionModuleLoad = null;
	public delegate long Delegate_maExtensionFunctionLoad(int _module, int _index);
	public Delegate_maExtensionFunctionLoad maExtensionFunctionLoad = null;
	public delegate long Delegate_maFontLoadDefault(int _type, int _style, int _size);
	public Delegate_maFontLoadDefault maFontLoadDefault = null;
	public delegate long Delegate_maFontSetCurrent(int _font);
	public Delegate_maFontSetCurrent maFontSetCurrent = null;
	public delegate long Delegate_maFontGetCount();
	public Delegate_maFontGetCount maFontGetCount = null;
	public delegate long Delegate_maFontGetName(int _index, int _buffer, int _bufferLen);
	public Delegate_maFontGetName maFontGetName = null;
	public delegate long Delegate_maFontLoadWithName(int _postScriptName, int _size);
	public Delegate_maFontLoadWithName maFontLoadWithName = null;
	public delegate long Delegate_maFontDelete(int _font);
	public Delegate_maFontDelete maFontDelete = null;
	public delegate long Delegate_maReportCallStack();
	public Delegate_maReportCallStack maReportCallStack = null;
	public delegate long Delegate_maDumpCallStackEx(int _str, int _data);
	public Delegate_maDumpCallStackEx maDumpCallStackEx = null;
	public delegate long Delegate_maProtectMemory(int _start, int _length);
	public Delegate_maProtectMemory maProtectMemory = null;
	public delegate long Delegate_maUnprotectMemory(int _start, int _length);
	public Delegate_maUnprotectMemory maUnprotectMemory = null;
	public delegate long Delegate_maSetMemoryProtection(int _enable);
	public Delegate_maSetMemoryProtection maSetMemoryProtection = null;
	public delegate long Delegate_maGetMemoryProtection();
	public Delegate_maGetMemoryProtection maGetMemoryProtection = null;
	public delegate long Delegate_maGetBatteryCharge();
	public Delegate_maGetBatteryCharge maGetBatteryCharge = null;
	public delegate long Delegate_maStartSpeaking(int _text);
	public Delegate_maStartSpeaking maStartSpeaking = null;
	public delegate long Delegate_maStopSpeaking();
	public Delegate_maStopSpeaking maStopSpeaking = null;
	public delegate long Delegate_maIsSpeaking();
	public Delegate_maIsSpeaking maIsSpeaking = null;
	public delegate long Delegate_maSetSpeechVolume(int _volume);
	public Delegate_maSetSpeechVolume maSetSpeechVolume = null;
	public delegate long Delegate_maSetSpeechSpeed(int _speed);
	public Delegate_maSetSpeechSpeed maSetSpeechSpeed = null;
	public delegate long Delegate_maSetSpeechPitch(int _pitch);
	public Delegate_maSetSpeechPitch maSetSpeechPitch = null;
	public delegate long Delegate_sinh(double _x);
	public Delegate_sinh sinh = null;
	public delegate long Delegate_cosh(double _x);
	public Delegate_cosh cosh = null;
	public delegate long Delegate_atanh(double _x);
	public Delegate_atanh atanh = null;
	public delegate long Delegate_maLockKeypad();
	public Delegate_maLockKeypad maLockKeypad = null;
	public delegate long Delegate_maUnlockKeypad();
	public Delegate_maUnlockKeypad maUnlockKeypad = null;
	public delegate long Delegate_maKeypadIsLocked();
	public Delegate_maKeypadIsLocked maKeypadIsLocked = null;
	public delegate long Delegate_maWriteLog(int _src, int _size);
	public Delegate_maWriteLog maWriteLog = null;
	public delegate long Delegate_maBtStartDeviceDiscovery(int _names);
	public Delegate_maBtStartDeviceDiscovery maBtStartDeviceDiscovery = null;
	public delegate long Delegate_maBtGetNewDevice(int _d);
	public Delegate_maBtGetNewDevice maBtGetNewDevice = null;
	public delegate long Delegate_maBtStartServiceDiscovery(int _address, int _uuid);
	public Delegate_maBtStartServiceDiscovery maBtStartServiceDiscovery = null;
	public delegate long Delegate_maBtGetNextServiceSize(int _dst);
	public Delegate_maBtGetNextServiceSize maBtGetNextServiceSize = null;
	public delegate long Delegate_maBtGetNewService(int _dst);
	public Delegate_maBtGetNewService maBtGetNewService = null;
	public delegate long Delegate_maBtCancelDiscovery();
	public Delegate_maBtCancelDiscovery maBtCancelDiscovery = null;
	public delegate long Delegate_maWlanStartDiscovery();
	public Delegate_maWlanStartDiscovery maWlanStartDiscovery = null;
	public delegate long Delegate_maWlanGetNewAp(int _dst);
	public Delegate_maWlanGetNewAp maWlanGetNewAp = null;
	public delegate long Delegate_maLocationStart();
	public Delegate_maLocationStart maLocationStart = null;
	public delegate long Delegate_maLocationStop();
	public Delegate_maLocationStop maLocationStop = null;
	public delegate long Delegate_maPlatformRequest(int _url);
	public Delegate_maPlatformRequest maPlatformRequest = null;
	public delegate long Delegate_maFileOpen(int _path, int _mode);
	public Delegate_maFileOpen maFileOpen = null;
	public delegate long Delegate_maFileExists(int _file);
	public Delegate_maFileExists maFileExists = null;
	public delegate long Delegate_maFileClose(int _file);
	public Delegate_maFileClose maFileClose = null;
	public delegate long Delegate_maFileCreate(int _file);
	public Delegate_maFileCreate maFileCreate = null;
	public delegate long Delegate_maFileDelete(int _file);
	public Delegate_maFileDelete maFileDelete = null;
	public delegate long Delegate_maFileSize(int _file);
	public Delegate_maFileSize maFileSize = null;
	public delegate long Delegate_maFileAvailableSpace(int _file);
	public Delegate_maFileAvailableSpace maFileAvailableSpace = null;
	public delegate long Delegate_maFileTotalSpace(int _file);
	public Delegate_maFileTotalSpace maFileTotalSpace = null;
	public delegate long Delegate_maFileDate(int _file);
	public Delegate_maFileDate maFileDate = null;
	public delegate long Delegate_maFileRename(int _file, int _newName);
	public Delegate_maFileRename maFileRename = null;
	public delegate long Delegate_maFileTruncate(int _file, int _offset);
	public Delegate_maFileTruncate maFileTruncate = null;
	public delegate long Delegate_maFileWrite(int _file, int _src, int _len);
	public Delegate_maFileWrite maFileWrite = null;
	public delegate long Delegate_maFileWriteFromData(int _file, int _data, int _offset, int _len);
	public Delegate_maFileWriteFromData maFileWriteFromData = null;
	public delegate long Delegate_maFileRead(int _file, int _dst, int _len);
	public Delegate_maFileRead maFileRead = null;
	public delegate long Delegate_maFileReadToData(int _file, int _data, int _offset, int _len);
	public Delegate_maFileReadToData maFileReadToData = null;
	public delegate long Delegate_maFileTell(int _file);
	public Delegate_maFileTell maFileTell = null;
	public delegate long Delegate_maFileSeek(int _file, int _offset, int _whence);
	public Delegate_maFileSeek maFileSeek = null;
	public delegate long Delegate_maFileListStart(int _path, int _filter, int _sorting);
	public Delegate_maFileListStart maFileListStart = null;
	public delegate long Delegate_maFileListNext(int _list, int _nameBuf, int _bufSize);
	public Delegate_maFileListNext maFileListNext = null;
	public delegate long Delegate_maFileListClose(int _list);
	public Delegate_maFileListClose maFileListClose = null;
	public delegate long Delegate_maSendTextSMS(int _dst, int _msg);
	public Delegate_maSendTextSMS maSendTextSMS = null;
	public delegate long Delegate_maRecordSetup();
	public Delegate_maRecordSetup maRecordSetup = null;
	public delegate long Delegate_maRecordStart(int _rec);
	public Delegate_maRecordStart maRecordStart = null;
	public delegate long Delegate_maRecordPause(int _rec);
	public Delegate_maRecordPause maRecordPause = null;
	public delegate long Delegate_maGetCellInfo(int _pInfo);
	public Delegate_maGetCellInfo maGetCellInfo = null;
	public delegate long Delegate_maCallDial(int _phoneNumber);
	public Delegate_maCallDial maCallDial = null;
	public delegate long Delegate_maCallAnswer();
	public Delegate_maCallAnswer maCallAnswer = null;
	public delegate long Delegate_maCallHangup();
	public Delegate_maCallHangup maCallHangup = null;
	public delegate long Delegate_maSoundPlayConn(int _conn, int _mimeType);
	public Delegate_maSoundPlayConn maSoundPlayConn = null;
	public delegate long Delegate_maStreamVideoStart(int _url);
	public Delegate_maStreamVideoStart maStreamVideoStart = null;
	public delegate long Delegate_maStreamVideoSize(int _stream);
	public Delegate_maStreamVideoSize maStreamVideoSize = null;
	public delegate long Delegate_maStreamVideoSetFrame(int _stream, int _rect);
	public Delegate_maStreamVideoSetFrame maStreamVideoSetFrame = null;
	public delegate long Delegate_maStreamLength(int _stream);
	public Delegate_maStreamLength maStreamLength = null;
	public delegate long Delegate_maStreamPos(int _stream);
	public Delegate_maStreamPos maStreamPos = null;
	public delegate long Delegate_maStreamSetPos(int _stream, int _pos);
	public Delegate_maStreamSetPos maStreamSetPos = null;
	public delegate long Delegate_maStreamPause(int _stream);
	public Delegate_maStreamPause maStreamPause = null;
	public delegate long Delegate_maStreamResume(int _stream);
	public Delegate_maStreamResume maStreamResume = null;
	public delegate long Delegate_maStreamClose(int _stream);
	public Delegate_maStreamClose maStreamClose = null;
	public delegate long Delegate_maFrameBufferGetInfo(int _info);
	public Delegate_maFrameBufferGetInfo maFrameBufferGetInfo = null;
	public delegate long Delegate_maFrameBufferInit(int _data);
	public Delegate_maFrameBufferInit maFrameBufferInit = null;
	public delegate long Delegate_maFrameBufferClose();
	public Delegate_maFrameBufferClose maFrameBufferClose = null;
	public delegate long Delegate_maAudioBufferInit(int _info);
	public Delegate_maAudioBufferInit maAudioBufferInit = null;
	public delegate long Delegate_maAudioBufferReady();
	public Delegate_maAudioBufferReady maAudioBufferReady = null;
	public delegate long Delegate_maAudioBufferClose();
	public Delegate_maAudioBufferClose maAudioBufferClose = null;
	public delegate long Delegate_maAccept(int _serv);
	public Delegate_maAccept maAccept = null;
	public delegate long Delegate_maSendToBackground();
	public Delegate_maSendToBackground maSendToBackground = null;
	public delegate long Delegate_maBringToForeground();
	public Delegate_maBringToForeground maBringToForeground = null;
	public delegate long Delegate_maAddDataSize(int _data, int _size);
	public Delegate_maAddDataSize maAddDataSize = null;
	public delegate long Delegate_maGetSystemProperty(int _key, int _buf, int _size);
	public Delegate_maGetSystemProperty maGetSystemProperty = null;
	public delegate long Delegate_maIapSave();
	public Delegate_maIapSave maIapSave = null;
	public delegate long Delegate_maIapReset();
	public Delegate_maIapReset maIapReset = null;
	public delegate long Delegate_maIapShutdown();
	public Delegate_maIapShutdown maIapShutdown = null;
	public delegate long Delegate_maNetworkStatus();
	public Delegate_maNetworkStatus maNetworkStatus = null;
	public delegate long Delegate_maIapSetMethod(int _type);
	public Delegate_maIapSetMethod maIapSetMethod = null;
	public delegate long Delegate_maIapSetFilter(int _filter);
	public Delegate_maIapSetFilter maIapSetFilter = null;
	public delegate long Delegate_maPimListOpen(int _listType);
	public Delegate_maPimListOpen maPimListOpen = null;
	public delegate long Delegate_maPimListNext(int _list);
	public Delegate_maPimListNext maPimListNext = null;
	public delegate long Delegate_maPimListClose(int _list);
	public Delegate_maPimListClose maPimListClose = null;
	public delegate long Delegate_maPimItemCount(int _item);
	public Delegate_maPimItemCount maPimItemCount = null;
	public delegate long Delegate_maPimItemGetField(int _item, int _n);
	public Delegate_maPimItemGetField maPimItemGetField = null;
	public delegate long Delegate_maPimItemFieldCount(int _item, int _field);
	public Delegate_maPimItemFieldCount maPimItemFieldCount = null;
	public delegate long Delegate_maPimItemGetAttributes(int _item, int _field, int _index);
	public Delegate_maPimItemGetAttributes maPimItemGetAttributes = null;
	public delegate long Delegate_maPimItemSetLabel(int _args, int _index);
	public Delegate_maPimItemSetLabel maPimItemSetLabel = null;
	public delegate long Delegate_maPimItemGetLabel(int _args, int _index);
	public Delegate_maPimItemGetLabel maPimItemGetLabel = null;
	public delegate long Delegate_maPimFieldType(int _list, int _field);
	public Delegate_maPimFieldType maPimFieldType = null;
	public delegate long Delegate_maPimItemGetValue(int _args, int _index);
	public Delegate_maPimItemGetValue maPimItemGetValue = null;
	public delegate long Delegate_maPimItemSetValue(int _args, int _index, int _attributes);
	public Delegate_maPimItemSetValue maPimItemSetValue = null;
	public delegate long Delegate_maPimItemAddValue(int _args, int _attributes);
	public Delegate_maPimItemAddValue maPimItemAddValue = null;
	public delegate long Delegate_maPimItemRemoveValue(int _item, int _field, int _index);
	public Delegate_maPimItemRemoveValue maPimItemRemoveValue = null;
	public delegate long Delegate_maPimItemClose(int _item);
	public Delegate_maPimItemClose maPimItemClose = null;
	public delegate long Delegate_maPimItemCreate(int _list);
	public Delegate_maPimItemCreate maPimItemCreate = null;
	public delegate long Delegate_maPimItemRemove(int _list, int _item);
	public Delegate_maPimItemRemove maPimItemRemove = null;
	public delegate long Delegate_maWidgetCreate(int _widgetType);
	public Delegate_maWidgetCreate maWidgetCreate = null;
	public delegate long Delegate_maWidgetDestroy(int _widget);
	public Delegate_maWidgetDestroy maWidgetDestroy = null;
	public delegate long Delegate_maWidgetAddChild(int _parent, int _child);
	public Delegate_maWidgetAddChild maWidgetAddChild = null;
	public delegate long Delegate_maWidgetInsertChild(int _parent, int _child, int _index);
	public Delegate_maWidgetInsertChild maWidgetInsertChild = null;
	public delegate long Delegate_maWidgetRemoveChild(int _child);
	public Delegate_maWidgetRemoveChild maWidgetRemoveChild = null;
	public delegate long Delegate_maWidgetModalDialogShow(int _dialogHandle);
	public Delegate_maWidgetModalDialogShow maWidgetModalDialogShow = null;
	public delegate long Delegate_maWidgetModalDialogHide(int _dialogHandle);
	public Delegate_maWidgetModalDialogHide maWidgetModalDialogHide = null;
	public delegate long Delegate_maWidgetScreenShow(int _screenHandle);
	public Delegate_maWidgetScreenShow maWidgetScreenShow = null;
	public delegate long Delegate_maWidgetStackScreenPush(int _stackScreen, int _newScreen);
	public Delegate_maWidgetStackScreenPush maWidgetStackScreenPush = null;
	public delegate long Delegate_maWidgetStackScreenPop(int _stackScreen);
	public Delegate_maWidgetStackScreenPop maWidgetStackScreenPop = null;
	public delegate long Delegate_maWidgetSetProperty(int _widget, int _property, int _value);
	public Delegate_maWidgetSetProperty maWidgetSetProperty = null;
	public delegate long Delegate_maWidgetGetProperty(int _widget, int _property, int _value, int _bufSize);
	public Delegate_maWidgetGetProperty maWidgetGetProperty = null;
	public delegate long Delegate_maWidgetScreenAddOptionsMenuItem(int _widget, int _title, int _iconPath, int _iconPredefined);
	public Delegate_maWidgetScreenAddOptionsMenuItem maWidgetScreenAddOptionsMenuItem = null;
	public delegate long Delegate_maCameraFormatNumber();
	public Delegate_maCameraFormatNumber maCameraFormatNumber = null;
	public delegate long Delegate_maCameraFormat(int _index, int _fmt);
	public Delegate_maCameraFormat maCameraFormat = null;
	public delegate long Delegate_maCameraStart();
	public Delegate_maCameraStart maCameraStart = null;
	public delegate long Delegate_maCameraStop();
	public Delegate_maCameraStop maCameraStop = null;
	public delegate long Delegate_maCameraSetPreview(int _widgetHandle);
	public Delegate_maCameraSetPreview maCameraSetPreview = null;
	public delegate long Delegate_maCameraSelect(int _cameraNumber);
	public Delegate_maCameraSelect maCameraSelect = null;
	public delegate long Delegate_maCameraNumber();
	public Delegate_maCameraNumber maCameraNumber = null;
	public delegate long Delegate_maCameraSnapshot(int _formatIndex, int _placeholder);
	public Delegate_maCameraSnapshot maCameraSnapshot = null;
	public delegate long Delegate_maCameraRecord(int _stopStartFlag);
	public Delegate_maCameraRecord maCameraRecord = null;
	public delegate long Delegate_maCameraSetProperty(int _property, int _value);
	public Delegate_maCameraSetProperty maCameraSetProperty = null;
	public delegate long Delegate_maCameraGetProperty(int _property, int _value, int _bufSize);
	public Delegate_maCameraGetProperty maCameraGetProperty = null;
	public delegate long Delegate_maCameraPreviewSize();
	public Delegate_maCameraPreviewSize maCameraPreviewSize = null;
	public delegate long Delegate_maCameraPreviewEventEnable(int _previewEventType, int _previewBuffer, int _previewArea);
	public Delegate_maCameraPreviewEventEnable maCameraPreviewEventEnable = null;
	public delegate long Delegate_maCameraPreviewEventDisable();
	public Delegate_maCameraPreviewEventDisable maCameraPreviewEventDisable = null;
	public delegate long Delegate_maCameraPreviewEventConsumed();
	public Delegate_maCameraPreviewEventConsumed maCameraPreviewEventConsumed = null;
	public delegate long Delegate_maShowVirtualKeyboard();
	public Delegate_maShowVirtualKeyboard maShowVirtualKeyboard = null;
	public delegate long Delegate_maTextBox(int _title, int _inText, int _outText, int _maxSize, int _constraints);
	public Delegate_maTextBox maTextBox = null;
	public delegate long Delegate_maKeyCaptureStart(int _keyCode);
	public Delegate_maKeyCaptureStart maKeyCaptureStart = null;
	public delegate long Delegate_maKeyCaptureStop();
	public Delegate_maKeyCaptureStop maKeyCaptureStop = null;
	public delegate long Delegate_maHomeScreenEventsOn();
	public Delegate_maHomeScreenEventsOn maHomeScreenEventsOn = null;
	public delegate long Delegate_maHomeScreenEventsOff();
	public Delegate_maHomeScreenEventsOff maHomeScreenEventsOff = null;
	public delegate long Delegate_maHomeScreenShortcutAdd(int _name);
	public Delegate_maHomeScreenShortcutAdd maHomeScreenShortcutAdd = null;
	public delegate long Delegate_maHomeScreenShortcutRemove(int _name);
	public Delegate_maHomeScreenShortcutRemove maHomeScreenShortcutRemove = null;
	public delegate long Delegate_maNotificationAdd(int _type, int _id, int _title, int _text);
	public Delegate_maNotificationAdd maNotificationAdd = null;
	public delegate long Delegate_maNotificationRemove(int _id);
	public Delegate_maNotificationRemove maNotificationRemove = null;
	public delegate long Delegate_maScreenSetOrientation(int _orientation);
	public Delegate_maScreenSetOrientation maScreenSetOrientation = null;
	public delegate long Delegate_maScreenSetFullscreen(int _fullscreen);
	public Delegate_maScreenSetFullscreen maScreenSetFullscreen = null;
	public delegate long Delegate_maAutostartOn();
	public Delegate_maAutostartOn maAutostartOn = null;
	public delegate long Delegate_maAutostartOff();
	public Delegate_maAutostartOff maAutostartOff = null;
	public delegate long Delegate_maScreenSetSupportedOrientations(int _orientations);
	public Delegate_maScreenSetSupportedOrientations maScreenSetSupportedOrientations = null;
	public delegate long Delegate_maScreenGetSupportedOrientations();
	public Delegate_maScreenGetSupportedOrientations maScreenGetSupportedOrientations = null;
	public delegate long Delegate_maScreenGetCurrentOrientation();
	public Delegate_maScreenGetCurrentOrientation maScreenGetCurrentOrientation = null;
	public delegate long Delegate_maScreenStateEventsOn();
	public Delegate_maScreenStateEventsOn maScreenStateEventsOn = null;
	public delegate long Delegate_maScreenStateEventsOff();
	public Delegate_maScreenStateEventsOff maScreenStateEventsOff = null;
	public delegate long Delegate_maWakeLock(int _flag);
	public Delegate_maWakeLock maWakeLock = null;
	public delegate long Delegate_maOpenGLInitFullscreen(int _glApi);
	public Delegate_maOpenGLInitFullscreen maOpenGLInitFullscreen = null;
	public delegate long Delegate_maOpenGLCloseFullscreen();
	public Delegate_maOpenGLCloseFullscreen maOpenGLCloseFullscreen = null;
	public delegate long Delegate_maOpenGLTexImage2D(int _image);
	public Delegate_maOpenGLTexImage2D maOpenGLTexImage2D = null;
	public delegate long Delegate_maOpenGLTexSubImage2D(int _image);
	public Delegate_maOpenGLTexSubImage2D maOpenGLTexSubImage2D = null;
	public delegate long Delegate_glActiveTexture(int _texture);
	public Delegate_glActiveTexture glActiveTexture = null;
	public delegate long Delegate_glBindBuffer(int _target, int _buffer);
	public Delegate_glBindBuffer glBindBuffer = null;
	public delegate long Delegate_glBindTexture(int _target, int _texture);
	public Delegate_glBindTexture glBindTexture = null;
	public delegate long Delegate_glBlendFunc(int _sfactor, int _dfactor);
	public Delegate_glBlendFunc glBlendFunc = null;
	public delegate long Delegate_glBufferData(int _target, int _size, int _data, int _usage);
	public Delegate_glBufferData glBufferData = null;
	public delegate long Delegate_glBufferSubData(int _target, int _offset, int _size, int _data);
	public Delegate_glBufferSubData glBufferSubData = null;
	public delegate long Delegate_glClear(int _mask);
	public Delegate_glClear glClear = null;
	public delegate long Delegate_glClearColor(float _red, float _green, float _blue, float _alpha);
	public Delegate_glClearColor glClearColor = null;
	public delegate long Delegate_glClearDepthf(float _depth);
	public Delegate_glClearDepthf glClearDepthf = null;
	public delegate long Delegate_glClearStencil(int _s);
	public Delegate_glClearStencil glClearStencil = null;
	public delegate long Delegate_glColorMask(int _red, int _green, int _blue, int _alpha);
	public Delegate_glColorMask glColorMask = null;
	public delegate long Delegate_glCompressedTexImage2D(int _target, int _level, int _internalformat, int _width, int _height, int _border, int _imageSize, int _data);
	public Delegate_glCompressedTexImage2D glCompressedTexImage2D = null;
	public delegate long Delegate_glCompressedTexSubImage2D(int _target, int _level, int _xoffset, int _yoffset, int _width, int _height, int _format, int _imageSize, int _data);
	public Delegate_glCompressedTexSubImage2D glCompressedTexSubImage2D = null;
	public delegate long Delegate_glCopyTexImage2D(int _target, int _level, int _internalformat, int _x, int _y, int _width, int _height, int _border);
	public Delegate_glCopyTexImage2D glCopyTexImage2D = null;
	public delegate long Delegate_glCopyTexSubImage2D(int _target, int _level, int _xoffset, int _yoffset, int _x, int _y, int _width, int _height);
	public Delegate_glCopyTexSubImage2D glCopyTexSubImage2D = null;
	public delegate long Delegate_glCullFace(int _mode);
	public Delegate_glCullFace glCullFace = null;
	public delegate long Delegate_glDeleteBuffers(int _n, int _buffers);
	public Delegate_glDeleteBuffers glDeleteBuffers = null;
	public delegate long Delegate_glDeleteTextures(int _n, int _textures);
	public Delegate_glDeleteTextures glDeleteTextures = null;
	public delegate long Delegate_glDepthFunc(int _func);
	public Delegate_glDepthFunc glDepthFunc = null;
	public delegate long Delegate_glDepthMask(int _flag);
	public Delegate_glDepthMask glDepthMask = null;
	public delegate long Delegate_glDepthRangef(float _zNear, float _zFar);
	public Delegate_glDepthRangef glDepthRangef = null;
	public delegate long Delegate_glDisable(int _cap);
	public Delegate_glDisable glDisable = null;
	public delegate long Delegate_glDrawArrays(int _mode, int _first, int _count);
	public Delegate_glDrawArrays glDrawArrays = null;
	public delegate long Delegate_glDrawElements(int _mode, int _count, int _type, int _indices);
	public Delegate_glDrawElements glDrawElements = null;
	public delegate long Delegate_glEnable(int _cap);
	public Delegate_glEnable glEnable = null;
	public delegate long Delegate_glFinish();
	public Delegate_glFinish glFinish = null;
	public delegate long Delegate_glFlush();
	public Delegate_glFlush glFlush = null;
	public delegate long Delegate_glFrontFace(int _mode);
	public Delegate_glFrontFace glFrontFace = null;
	public delegate long Delegate_glGenBuffers(int _n, int _buffers);
	public Delegate_glGenBuffers glGenBuffers = null;
	public delegate long Delegate_glGenTextures(int _n, int _textures);
	public Delegate_glGenTextures glGenTextures = null;
	public delegate long Delegate_glGetBooleanv(int _pname, int _params);
	public Delegate_glGetBooleanv glGetBooleanv = null;
	public delegate long Delegate_glGetBufferParameteriv(int _target, int _pname, int _params);
	public Delegate_glGetBufferParameteriv glGetBufferParameteriv = null;
	public delegate long Delegate_glGetError();
	public Delegate_glGetError glGetError = null;
	public delegate long Delegate_glGetFloatv(int _pname, int _params);
	public Delegate_glGetFloatv glGetFloatv = null;
	public delegate long Delegate_glGetIntegerv(int _pname, int _params);
	public Delegate_glGetIntegerv glGetIntegerv = null;
	public delegate long Delegate_glGetStringHandle(int _name);
	public Delegate_glGetStringHandle glGetStringHandle = null;
	public delegate long Delegate_glGetTexParameterfv(int _target, int _pname, int _params);
	public Delegate_glGetTexParameterfv glGetTexParameterfv = null;
	public delegate long Delegate_glGetTexParameteriv(int _target, int _pname, int _params);
	public Delegate_glGetTexParameteriv glGetTexParameteriv = null;
	public delegate long Delegate_glHint(int _target, int _mode);
	public Delegate_glHint glHint = null;
	public delegate long Delegate_glIsBuffer(int _buffer);
	public Delegate_glIsBuffer glIsBuffer = null;
	public delegate long Delegate_glIsEnabled(int _cap);
	public Delegate_glIsEnabled glIsEnabled = null;
	public delegate long Delegate_glIsTexture(int _texture);
	public Delegate_glIsTexture glIsTexture = null;
	public delegate long Delegate_glLineWidth(float _width);
	public Delegate_glLineWidth glLineWidth = null;
	public delegate long Delegate_glPixelStorei(int _pname, int _param);
	public Delegate_glPixelStorei glPixelStorei = null;
	public delegate long Delegate_glPolygonOffset(float _factor, float _units);
	public Delegate_glPolygonOffset glPolygonOffset = null;
	public delegate long Delegate_glReadPixels(int _x, int _y, int _width, int _height, int _format, int _type, int _pixels);
	public Delegate_glReadPixels glReadPixels = null;
	public delegate long Delegate_glSampleCoverage(float _value, int _invert);
	public Delegate_glSampleCoverage glSampleCoverage = null;
	public delegate long Delegate_glScissor(int _x, int _y, int _width, int _height);
	public Delegate_glScissor glScissor = null;
	public delegate long Delegate_glStencilFunc(int _func, int _ref, int _mask);
	public Delegate_glStencilFunc glStencilFunc = null;
	public delegate long Delegate_glStencilMask(int _mask);
	public Delegate_glStencilMask glStencilMask = null;
	public delegate long Delegate_glStencilOp(int _fail, int _zfail, int _zpass);
	public Delegate_glStencilOp glStencilOp = null;
	public delegate long Delegate_glTexImage2D(int _target, int _level, int _internalformat, int _width, int _height, int _border, int _format, int _type, int _pixels);
	public Delegate_glTexImage2D glTexImage2D = null;
	public delegate long Delegate_glTexParameterf(int _target, int _pname, float _param);
	public Delegate_glTexParameterf glTexParameterf = null;
	public delegate long Delegate_glTexParameterfv(int _target, int _pname, int _params);
	public Delegate_glTexParameterfv glTexParameterfv = null;
	public delegate long Delegate_glTexParameteri(int _target, int _pname, int _param);
	public Delegate_glTexParameteri glTexParameteri = null;
	public delegate long Delegate_glTexParameteriv(int _target, int _pname, int _params);
	public Delegate_glTexParameteriv glTexParameteriv = null;
	public delegate long Delegate_glTexSubImage2D(int _target, int _level, int _xoffset, int _yoffset, int _width, int _height, int _format, int _type, int _pixels);
	public Delegate_glTexSubImage2D glTexSubImage2D = null;
	public delegate long Delegate_glViewport(int _x, int _y, int _width, int _height);
	public Delegate_glViewport glViewport = null;
	public delegate long Delegate_glAttachShader(int _program, int _shader);
	public Delegate_glAttachShader glAttachShader = null;
	public delegate long Delegate_glBindAttribLocation(int _program, int _index, int _name);
	public Delegate_glBindAttribLocation glBindAttribLocation = null;
	public delegate long Delegate_glBindFramebuffer(int _target, int _framebuffer);
	public Delegate_glBindFramebuffer glBindFramebuffer = null;
	public delegate long Delegate_glBindRenderbuffer(int _target, int _renderbuffer);
	public Delegate_glBindRenderbuffer glBindRenderbuffer = null;
	public delegate long Delegate_glBlendColor(float _red, float _green, float _blue, float _alpha);
	public Delegate_glBlendColor glBlendColor = null;
	public delegate long Delegate_glBlendEquation(int _mode);
	public Delegate_glBlendEquation glBlendEquation = null;
	public delegate long Delegate_glBlendEquationSeparate(int _modeRGB, int _modeAlpha);
	public Delegate_glBlendEquationSeparate glBlendEquationSeparate = null;
	public delegate long Delegate_glBlendFuncSeparate(int _srcRGB, int _dstRGB, int _srcAlpha, int _dstAlpha);
	public Delegate_glBlendFuncSeparate glBlendFuncSeparate = null;
	public delegate long Delegate_glCheckFramebufferStatus(int _target);
	public Delegate_glCheckFramebufferStatus glCheckFramebufferStatus = null;
	public delegate long Delegate_glCompileShader(int _shader);
	public Delegate_glCompileShader glCompileShader = null;
	public delegate long Delegate_glCreateProgram();
	public Delegate_glCreateProgram glCreateProgram = null;
	public delegate long Delegate_glCreateShader(int _type);
	public Delegate_glCreateShader glCreateShader = null;
	public delegate long Delegate_glDeleteFramebuffers(int _n, int _framebuffers);
	public Delegate_glDeleteFramebuffers glDeleteFramebuffers = null;
	public delegate long Delegate_glDeleteProgram(int _program);
	public Delegate_glDeleteProgram glDeleteProgram = null;
	public delegate long Delegate_glDeleteRenderbuffers(int _n, int _renderbuffers);
	public Delegate_glDeleteRenderbuffers glDeleteRenderbuffers = null;
	public delegate long Delegate_glDeleteShader(int _shader);
	public Delegate_glDeleteShader glDeleteShader = null;
	public delegate long Delegate_glDetachShader(int _program, int _shader);
	public Delegate_glDetachShader glDetachShader = null;
	public delegate long Delegate_glDisableVertexAttribArray(int _index);
	public Delegate_glDisableVertexAttribArray glDisableVertexAttribArray = null;
	public delegate long Delegate_glEnableVertexAttribArray(int _index);
	public Delegate_glEnableVertexAttribArray glEnableVertexAttribArray = null;
	public delegate long Delegate_glFramebufferRenderbuffer(int _target, int _attachment, int _renderbuffertarget, int _renderbuffer);
	public Delegate_glFramebufferRenderbuffer glFramebufferRenderbuffer = null;
	public delegate long Delegate_glFramebufferTexture2D(int _target, int _attachment, int _textarget, int _texture, int _level);
	public Delegate_glFramebufferTexture2D glFramebufferTexture2D = null;
	public delegate long Delegate_glGenerateMipmap(int _target);
	public Delegate_glGenerateMipmap glGenerateMipmap = null;
	public delegate long Delegate_glGenFramebuffers(int _n, int _framebuffers);
	public Delegate_glGenFramebuffers glGenFramebuffers = null;
	public delegate long Delegate_glGenRenderbuffers(int _n, int _renderbuffers);
	public Delegate_glGenRenderbuffers glGenRenderbuffers = null;
	public delegate long Delegate_glGetActiveAttrib(int _program, int _index, int _bufsize, int _length, int _size, int _type, int _name);
	public Delegate_glGetActiveAttrib glGetActiveAttrib = null;
	public delegate long Delegate_glGetActiveUniform(int _program, int _index, int _bufsize, int _length, int _size, int _type, int _name);
	public Delegate_glGetActiveUniform glGetActiveUniform = null;
	public delegate long Delegate_glGetAttachedShaders(int _program, int _maxcount, int _count, int _shaders);
	public Delegate_glGetAttachedShaders glGetAttachedShaders = null;
	public delegate long Delegate_glGetAttribLocation(int _program, int _name);
	public Delegate_glGetAttribLocation glGetAttribLocation = null;
	public delegate long Delegate_glGetFramebufferAttachmentParameteriv(int _target, int _attachment, int _pname, int _params);
	public Delegate_glGetFramebufferAttachmentParameteriv glGetFramebufferAttachmentParameteriv = null;
	public delegate long Delegate_glGetProgramiv(int _program, int _pname, int _params);
	public Delegate_glGetProgramiv glGetProgramiv = null;
	public delegate long Delegate_glGetProgramInfoLog(int _program, int _bufsize, int _length, int _infolog);
	public Delegate_glGetProgramInfoLog glGetProgramInfoLog = null;
	public delegate long Delegate_glGetRenderbufferParameteriv(int _target, int _pname, int _params);
	public Delegate_glGetRenderbufferParameteriv glGetRenderbufferParameteriv = null;
	public delegate long Delegate_glGetShaderiv(int _shader, int _pname, int _params);
	public Delegate_glGetShaderiv glGetShaderiv = null;
	public delegate long Delegate_glGetShaderInfoLog(int _shader, int _bufsize, int _length, int _infolog);
	public Delegate_glGetShaderInfoLog glGetShaderInfoLog = null;
	public delegate long Delegate_glGetShaderPrecisionFormat(int _shadertype, int _precisiontype, int _range, int _precision);
	public Delegate_glGetShaderPrecisionFormat glGetShaderPrecisionFormat = null;
	public delegate long Delegate_glGetShaderSource(int _shader, int _bufsize, int _length, int _source);
	public Delegate_glGetShaderSource glGetShaderSource = null;
	public delegate long Delegate_glGetUniformfv(int _program, int _location, int _params);
	public Delegate_glGetUniformfv glGetUniformfv = null;
	public delegate long Delegate_glGetUniformiv(int _program, int _location, int _params);
	public Delegate_glGetUniformiv glGetUniformiv = null;
	public delegate long Delegate_glGetUniformLocation(int _program, int _name);
	public Delegate_glGetUniformLocation glGetUniformLocation = null;
	public delegate long Delegate_glGetVertexAttribfv(int _index, int _pname, int _params);
	public Delegate_glGetVertexAttribfv glGetVertexAttribfv = null;
	public delegate long Delegate_glGetVertexAttribiv(int _index, int _pname, int _params);
	public Delegate_glGetVertexAttribiv glGetVertexAttribiv = null;
	public delegate long Delegate_glGetVertexAttribPointerv(int _index, int _pname, int _pointer);
	public Delegate_glGetVertexAttribPointerv glGetVertexAttribPointerv = null;
	public delegate long Delegate_glIsFramebuffer(int _framebuffer);
	public Delegate_glIsFramebuffer glIsFramebuffer = null;
	public delegate long Delegate_glIsProgram(int _program);
	public Delegate_glIsProgram glIsProgram = null;
	public delegate long Delegate_glIsRenderbuffer(int _renderbuffer);
	public Delegate_glIsRenderbuffer glIsRenderbuffer = null;
	public delegate long Delegate_glIsShader(int _shader);
	public Delegate_glIsShader glIsShader = null;
	public delegate long Delegate_glLinkProgram(int _program);
	public Delegate_glLinkProgram glLinkProgram = null;
	public delegate long Delegate_glReleaseShaderCompiler();
	public Delegate_glReleaseShaderCompiler glReleaseShaderCompiler = null;
	public delegate long Delegate_glRenderbufferStorage(int _target, int _internalformat, int _width, int _height);
	public Delegate_glRenderbufferStorage glRenderbufferStorage = null;
	public delegate long Delegate_glShaderBinary(int _n, int _shaders, int _binaryformat, int _binary, int _length);
	public Delegate_glShaderBinary glShaderBinary = null;
	public delegate long Delegate_glShaderSource(int _shader, int _count, int _string, int _length);
	public Delegate_glShaderSource glShaderSource = null;
	public delegate long Delegate_glStencilFuncSeparate(int _face, int _func, int _ref, int _mask);
	public Delegate_glStencilFuncSeparate glStencilFuncSeparate = null;
	public delegate long Delegate_glStencilMaskSeparate(int _face, int _mask);
	public Delegate_glStencilMaskSeparate glStencilMaskSeparate = null;
	public delegate long Delegate_glStencilOpSeparate(int _face, int _fail, int _zfail, int _zpass);
	public Delegate_glStencilOpSeparate glStencilOpSeparate = null;
	public delegate long Delegate_glUniform1f(int _location, float _x);
	public Delegate_glUniform1f glUniform1f = null;
	public delegate long Delegate_glUniform1fv(int _location, int _count, int _v);
	public Delegate_glUniform1fv glUniform1fv = null;
	public delegate long Delegate_glUniform1i(int _location, int _x);
	public Delegate_glUniform1i glUniform1i = null;
	public delegate long Delegate_glUniform1iv(int _location, int _count, int _v);
	public Delegate_glUniform1iv glUniform1iv = null;
	public delegate long Delegate_glUniform2f(int _location, float _x, float _y);
	public Delegate_glUniform2f glUniform2f = null;
	public delegate long Delegate_glUniform2fv(int _location, int _count, int _v);
	public Delegate_glUniform2fv glUniform2fv = null;
	public delegate long Delegate_glUniform2i(int _location, int _x, int _y);
	public Delegate_glUniform2i glUniform2i = null;
	public delegate long Delegate_glUniform2iv(int _location, int _count, int _v);
	public Delegate_glUniform2iv glUniform2iv = null;
	public delegate long Delegate_glUniform3f(int _location, float _x, float _y, float _z);
	public Delegate_glUniform3f glUniform3f = null;
	public delegate long Delegate_glUniform3fv(int _location, int _count, int _v);
	public Delegate_glUniform3fv glUniform3fv = null;
	public delegate long Delegate_glUniform3i(int _location, int _x, int _y, int _z);
	public Delegate_glUniform3i glUniform3i = null;
	public delegate long Delegate_glUniform3iv(int _location, int _count, int _v);
	public Delegate_glUniform3iv glUniform3iv = null;
	public delegate long Delegate_glUniform4f(int _location, float _x, float _y, float _z, float _w);
	public Delegate_glUniform4f glUniform4f = null;
	public delegate long Delegate_glUniform4fv(int _location, int _count, int _v);
	public Delegate_glUniform4fv glUniform4fv = null;
	public delegate long Delegate_glUniform4i(int _location, int _x, int _y, int _z, int _w);
	public Delegate_glUniform4i glUniform4i = null;
	public delegate long Delegate_glUniform4iv(int _location, int _count, int _v);
	public Delegate_glUniform4iv glUniform4iv = null;
	public delegate long Delegate_glUniformMatrix2fv(int _location, int _count, int _transpose, int _value);
	public Delegate_glUniformMatrix2fv glUniformMatrix2fv = null;
	public delegate long Delegate_glUniformMatrix3fv(int _location, int _count, int _transpose, int _value);
	public Delegate_glUniformMatrix3fv glUniformMatrix3fv = null;
	public delegate long Delegate_glUniformMatrix4fv(int _location, int _count, int _transpose, int _value);
	public Delegate_glUniformMatrix4fv glUniformMatrix4fv = null;
	public delegate long Delegate_glUseProgram(int _program);
	public Delegate_glUseProgram glUseProgram = null;
	public delegate long Delegate_glValidateProgram(int _program);
	public Delegate_glValidateProgram glValidateProgram = null;
	public delegate long Delegate_glVertexAttrib1f(int _indx, float _x);
	public Delegate_glVertexAttrib1f glVertexAttrib1f = null;
	public delegate long Delegate_glVertexAttrib1fv(int _indx, int _values);
	public Delegate_glVertexAttrib1fv glVertexAttrib1fv = null;
	public delegate long Delegate_glVertexAttrib2f(int _indx, float _x, float _y);
	public Delegate_glVertexAttrib2f glVertexAttrib2f = null;
	public delegate long Delegate_glVertexAttrib2fv(int _indx, int _values);
	public Delegate_glVertexAttrib2fv glVertexAttrib2fv = null;
	public delegate long Delegate_glVertexAttrib3f(int _indx, float _x, float _y, float _z);
	public Delegate_glVertexAttrib3f glVertexAttrib3f = null;
	public delegate long Delegate_glVertexAttrib3fv(int _indx, int _values);
	public Delegate_glVertexAttrib3fv glVertexAttrib3fv = null;
	public delegate long Delegate_glVertexAttrib4f(int _indx, float _x, float _y, float _z, float _w);
	public Delegate_glVertexAttrib4f glVertexAttrib4f = null;
	public delegate long Delegate_glVertexAttrib4fv(int _indx, int _values);
	public Delegate_glVertexAttrib4fv glVertexAttrib4fv = null;
	public delegate long Delegate_glVertexAttribPointer(int _indx, int _size, int _type, int _normalized, int _stride, int _ptr);
	public Delegate_glVertexAttribPointer glVertexAttribPointer = null;
	public delegate long Delegate_glAlphaFunc(int _func, float _ref);
	public Delegate_glAlphaFunc glAlphaFunc = null;
	public delegate long Delegate_glClipPlanef(int _plane, int _equation);
	public Delegate_glClipPlanef glClipPlanef = null;
	public delegate long Delegate_glColor4f(float _red, float _green, float _blue, float _alpha);
	public Delegate_glColor4f glColor4f = null;
	public delegate long Delegate_glFogf(int _pname, float _param);
	public Delegate_glFogf glFogf = null;
	public delegate long Delegate_glFogfv(int _pname, int _params);
	public Delegate_glFogfv glFogfv = null;
	public delegate long Delegate_glFrustumf(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar);
	public Delegate_glFrustumf glFrustumf = null;
	public delegate long Delegate_glGetClipPlanef(int _pname, int _eqn);
	public Delegate_glGetClipPlanef glGetClipPlanef = null;
	public delegate long Delegate_glGetLightfv(int _light, int _pname, int _params);
	public Delegate_glGetLightfv glGetLightfv = null;
	public delegate long Delegate_glGetMaterialfv(int _face, int _pname, int _params);
	public Delegate_glGetMaterialfv glGetMaterialfv = null;
	public delegate long Delegate_glGetTexEnvfv(int _env, int _pname, int _params);
	public Delegate_glGetTexEnvfv glGetTexEnvfv = null;
	public delegate long Delegate_glLightModelf(int _pname, float _param);
	public Delegate_glLightModelf glLightModelf = null;
	public delegate long Delegate_glLightModelfv(int _pname, int _params);
	public Delegate_glLightModelfv glLightModelfv = null;
	public delegate long Delegate_glLightf(int _light, int _pname, float _param);
	public Delegate_glLightf glLightf = null;
	public delegate long Delegate_glLightfv(int _light, int _pname, int _params);
	public Delegate_glLightfv glLightfv = null;
	public delegate long Delegate_glLoadMatrixf(int _m);
	public Delegate_glLoadMatrixf glLoadMatrixf = null;
	public delegate long Delegate_glMaterialf(int _face, int _pname, float _param);
	public Delegate_glMaterialf glMaterialf = null;
	public delegate long Delegate_glMaterialfv(int _face, int _pname, int _params);
	public Delegate_glMaterialfv glMaterialfv = null;
	public delegate long Delegate_glMultMatrixf(int _m);
	public Delegate_glMultMatrixf glMultMatrixf = null;
	public delegate long Delegate_glMultiTexCoord4f(int _target, float _s, float _t, float _r, float _q);
	public Delegate_glMultiTexCoord4f glMultiTexCoord4f = null;
	public delegate long Delegate_glNormal3f(float _nx, float _ny, float _nz);
	public Delegate_glNormal3f glNormal3f = null;
	public delegate long Delegate_glOrthof(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar);
	public Delegate_glOrthof glOrthof = null;
	public delegate long Delegate_glPointParameterf(int _pname, float _param);
	public Delegate_glPointParameterf glPointParameterf = null;
	public delegate long Delegate_glPointParameterfv(int _pname, int _params);
	public Delegate_glPointParameterfv glPointParameterfv = null;
	public delegate long Delegate_glPointSize(float _size);
	public Delegate_glPointSize glPointSize = null;
	public delegate long Delegate_glRotatef(float _angle, float _x, float _y, float _z);
	public Delegate_glRotatef glRotatef = null;
	public delegate long Delegate_glScalef(float _x, float _y, float _z);
	public Delegate_glScalef glScalef = null;
	public delegate long Delegate_glTexEnvf(int _target, int _pname, float _param);
	public Delegate_glTexEnvf glTexEnvf = null;
	public delegate long Delegate_glTexEnvfv(int _target, int _pname, int _params);
	public Delegate_glTexEnvfv glTexEnvfv = null;
	public delegate long Delegate_glTranslatef(float _x, float _y, float _z);
	public Delegate_glTranslatef glTranslatef = null;
	public delegate long Delegate_glAlphaFuncx(int _func, int _ref);
	public Delegate_glAlphaFuncx glAlphaFuncx = null;
	public delegate long Delegate_glClearColorx(int _red, int _green, int _blue, int _alpha);
	public Delegate_glClearColorx glClearColorx = null;
	public delegate long Delegate_glClearDepthx(int _depth);
	public Delegate_glClearDepthx glClearDepthx = null;
	public delegate long Delegate_glClientActiveTexture(int _texture);
	public Delegate_glClientActiveTexture glClientActiveTexture = null;
	public delegate long Delegate_glClipPlanex(int _plane, int _equation);
	public Delegate_glClipPlanex glClipPlanex = null;
	public delegate long Delegate_glColor4ub(int _red, int _green, int _blue, int _alpha);
	public Delegate_glColor4ub glColor4ub = null;
	public delegate long Delegate_glColor4x(int _red, int _green, int _blue, int _alpha);
	public Delegate_glColor4x glColor4x = null;
	public delegate long Delegate_glColorPointer(int _size, int _type, int _stride, int _pointer);
	public Delegate_glColorPointer glColorPointer = null;
	public delegate long Delegate_glDepthRangex(int _zNear, int _zFar);
	public Delegate_glDepthRangex glDepthRangex = null;
	public delegate long Delegate_glDisableClientState(int _array);
	public Delegate_glDisableClientState glDisableClientState = null;
	public delegate long Delegate_glEnableClientState(int _array);
	public Delegate_glEnableClientState glEnableClientState = null;
	public delegate long Delegate_glFogx(int _pname, int _param);
	public Delegate_glFogx glFogx = null;
	public delegate long Delegate_glFogxv(int _pname, int _params);
	public Delegate_glFogxv glFogxv = null;
	public delegate long Delegate_glFrustumx(int _left, int _right, int _bottom, int _top, int _zNear, int _zFar);
	public Delegate_glFrustumx glFrustumx = null;
	public delegate long Delegate_glGetClipPlanex(int _pname, int _eqn);
	public Delegate_glGetClipPlanex glGetClipPlanex = null;
	public delegate long Delegate_glGetFixedv(int _pname, int _params);
	public Delegate_glGetFixedv glGetFixedv = null;
	public delegate long Delegate_glGetLightxv(int _light, int _pname, int _params);
	public Delegate_glGetLightxv glGetLightxv = null;
	public delegate long Delegate_glGetMaterialxv(int _face, int _pname, int _params);
	public Delegate_glGetMaterialxv glGetMaterialxv = null;
	public delegate long Delegate_glGetPointerv(int _pname, int _params);
	public Delegate_glGetPointerv glGetPointerv = null;
	public delegate long Delegate_glGetTexEnviv(int _env, int _pname, int _params);
	public Delegate_glGetTexEnviv glGetTexEnviv = null;
	public delegate long Delegate_glGetTexEnvxv(int _env, int _pname, int _params);
	public Delegate_glGetTexEnvxv glGetTexEnvxv = null;
	public delegate long Delegate_glGetTexParameterxv(int _target, int _pname, int _params);
	public Delegate_glGetTexParameterxv glGetTexParameterxv = null;
	public delegate long Delegate_glLightModelx(int _pname, int _param);
	public Delegate_glLightModelx glLightModelx = null;
	public delegate long Delegate_glLightModelxv(int _pname, int _params);
	public Delegate_glLightModelxv glLightModelxv = null;
	public delegate long Delegate_glLightx(int _light, int _pname, int _param);
	public Delegate_glLightx glLightx = null;
	public delegate long Delegate_glLightxv(int _light, int _pname, int _params);
	public Delegate_glLightxv glLightxv = null;
	public delegate long Delegate_glLineWidthx(int _width);
	public Delegate_glLineWidthx glLineWidthx = null;
	public delegate long Delegate_glLoadIdentity();
	public Delegate_glLoadIdentity glLoadIdentity = null;
	public delegate long Delegate_glLoadMatrixx(int _m);
	public Delegate_glLoadMatrixx glLoadMatrixx = null;
	public delegate long Delegate_glLogicOp(int _opcode);
	public Delegate_glLogicOp glLogicOp = null;
	public delegate long Delegate_glMaterialx(int _face, int _pname, int _param);
	public Delegate_glMaterialx glMaterialx = null;
	public delegate long Delegate_glMaterialxv(int _face, int _pname, int _params);
	public Delegate_glMaterialxv glMaterialxv = null;
	public delegate long Delegate_glMatrixMode(int _mode);
	public Delegate_glMatrixMode glMatrixMode = null;
	public delegate long Delegate_glMultMatrixx(int _m);
	public Delegate_glMultMatrixx glMultMatrixx = null;
	public delegate long Delegate_glMultiTexCoord4x(int _target, int _s, int _t, int _r, int _q);
	public Delegate_glMultiTexCoord4x glMultiTexCoord4x = null;
	public delegate long Delegate_glNormal3x(int _nx, int _ny, int _nz);
	public Delegate_glNormal3x glNormal3x = null;
	public delegate long Delegate_glNormalPointer(int _type, int _stride, int _pointer);
	public Delegate_glNormalPointer glNormalPointer = null;
	public delegate long Delegate_glOrthox(int _left, int _right, int _bottom, int _top, int _zNear, int _zFar);
	public Delegate_glOrthox glOrthox = null;
	public delegate long Delegate_glPointParameterx(int _pname, int _param);
	public Delegate_glPointParameterx glPointParameterx = null;
	public delegate long Delegate_glPointParameterxv(int _pname, int _params);
	public Delegate_glPointParameterxv glPointParameterxv = null;
	public delegate long Delegate_glPointSizex(int _size);
	public Delegate_glPointSizex glPointSizex = null;
	public delegate long Delegate_glPolygonOffsetx(int _factor, int _units);
	public Delegate_glPolygonOffsetx glPolygonOffsetx = null;
	public delegate long Delegate_glPopMatrix();
	public Delegate_glPopMatrix glPopMatrix = null;
	public delegate long Delegate_glPushMatrix();
	public Delegate_glPushMatrix glPushMatrix = null;
	public delegate long Delegate_glRotatex(int _angle, int _x, int _y, int _z);
	public Delegate_glRotatex glRotatex = null;
	public delegate long Delegate_glSampleCoveragex(int _value, int _invert);
	public Delegate_glSampleCoveragex glSampleCoveragex = null;
	public delegate long Delegate_glScalex(int _x, int _y, int _z);
	public Delegate_glScalex glScalex = null;
	public delegate long Delegate_glShadeModel(int _mode);
	public Delegate_glShadeModel glShadeModel = null;
	public delegate long Delegate_glTexCoordPointer(int _size, int _type, int _stride, int _pointer);
	public Delegate_glTexCoordPointer glTexCoordPointer = null;
	public delegate long Delegate_glTexEnvi(int _target, int _pname, int _param);
	public Delegate_glTexEnvi glTexEnvi = null;
	public delegate long Delegate_glTexEnvx(int _target, int _pname, int _param);
	public Delegate_glTexEnvx glTexEnvx = null;
	public delegate long Delegate_glTexEnviv(int _target, int _pname, int _params);
	public Delegate_glTexEnviv glTexEnviv = null;
	public delegate long Delegate_glTexEnvxv(int _target, int _pname, int _params);
	public Delegate_glTexEnvxv glTexEnvxv = null;
	public delegate long Delegate_glTexParameterx(int _target, int _pname, int _param);
	public Delegate_glTexParameterx glTexParameterx = null;
	public delegate long Delegate_glTexParameterxv(int _target, int _pname, int _params);
	public Delegate_glTexParameterxv glTexParameterxv = null;
	public delegate long Delegate_glTranslatex(int _x, int _y, int _z);
	public Delegate_glTranslatex glTranslatex = null;
	public delegate long Delegate_glVertexPointer(int _size, int _type, int _stride, int _pointer);
	public Delegate_glVertexPointer glVertexPointer = null;
	public delegate long Delegate_glPointSizePointerOES(int _type, int _stride, int _pointer);
	public Delegate_glPointSizePointerOES glPointSizePointerOES = null;
	public delegate long Delegate_glIsRenderbufferOES(int _renderbuffer);
	public Delegate_glIsRenderbufferOES glIsRenderbufferOES = null;
	public delegate long Delegate_glBindRenderbufferOES(int _target, int _renderbuffer);
	public Delegate_glBindRenderbufferOES glBindRenderbufferOES = null;
	public delegate long Delegate_glDeleteRenderbuffersOES(int _n, int _renderbuffers);
	public Delegate_glDeleteRenderbuffersOES glDeleteRenderbuffersOES = null;
	public delegate long Delegate_glGenRenderbuffersOES(int _n, int _renderbuffers);
	public Delegate_glGenRenderbuffersOES glGenRenderbuffersOES = null;
	public delegate long Delegate_glRenderbufferStorageOES(int _target, int _internalformat, int _width, int _height);
	public Delegate_glRenderbufferStorageOES glRenderbufferStorageOES = null;
	public delegate long Delegate_glGetRenderbufferParameterivOES(int _target, int _pname, int _params);
	public Delegate_glGetRenderbufferParameterivOES glGetRenderbufferParameterivOES = null;
	public delegate long Delegate_glIsFramebufferOES(int _framebuffer);
	public Delegate_glIsFramebufferOES glIsFramebufferOES = null;
	public delegate long Delegate_glBindFramebufferOES(int _target, int _framebuffer);
	public Delegate_glBindFramebufferOES glBindFramebufferOES = null;
	public delegate long Delegate_glDeleteFramebuffersOES(int _n, int _framebuffers);
	public Delegate_glDeleteFramebuffersOES glDeleteFramebuffersOES = null;
	public delegate long Delegate_glGenFramebuffersOES(int _n, int _framebuffers);
	public Delegate_glGenFramebuffersOES glGenFramebuffersOES = null;
	public delegate long Delegate_glCheckFramebufferStatusOES(int _target);
	public Delegate_glCheckFramebufferStatusOES glCheckFramebufferStatusOES = null;
	public delegate long Delegate_glFramebufferRenderbufferOES(int _target, int _attachment, int _renderbuffertarget, int _renderbuffer);
	public Delegate_glFramebufferRenderbufferOES glFramebufferRenderbufferOES = null;
	public delegate long Delegate_glFramebufferTexture2DOES(int _target, int _attachment, int _textarget, int _texture, int _level);
	public Delegate_glFramebufferTexture2DOES glFramebufferTexture2DOES = null;
	public delegate long Delegate_glGetFramebufferAttachmentParameterivOES(int _target, int _attachment, int _pname, int _params);
	public Delegate_glGetFramebufferAttachmentParameterivOES glGetFramebufferAttachmentParameterivOES = null;
	public delegate long Delegate_glGenerateMipmapOES(int _target);
	public Delegate_glGenerateMipmapOES glGenerateMipmapOES = null;
	public delegate long Delegate_maReportResourceInformation();
	public Delegate_maReportResourceInformation maReportResourceInformation = null;
	public delegate long Delegate_maMessageBox(int _title, int _message);
	public Delegate_maMessageBox maMessageBox = null;
	public delegate long Delegate_maAlert(int _title, int _message, int _button1, int _button2, int _button3);
	public Delegate_maAlert maAlert = null;
	public delegate long Delegate_maImagePickerOpen();
	public Delegate_maImagePickerOpen maImagePickerOpen = null;
	public delegate long Delegate_maOptionsBox(int _title, int _destructiveButtonTitle, int _cancelButtonTitle, int _otherButtonTitles, int _otherButtonTitlesSize);
	public Delegate_maOptionsBox maOptionsBox = null;
	public delegate long Delegate_maSensorStart(int _sensor, int _interval);
	public Delegate_maSensorStart maSensorStart = null;
	public delegate long Delegate_maSensorStop(int _sensor);
	public Delegate_maSensorStop maSensorStop = null;
	public delegate long Delegate_maNFCStart();
	public Delegate_maNFCStart maNFCStart = null;
	public delegate long Delegate_maNFCStop();
	public Delegate_maNFCStop maNFCStop = null;
	public delegate long Delegate_maNFCReadTag(int _nfcContext);
	public Delegate_maNFCReadTag maNFCReadTag = null;
	public delegate long Delegate_maNFCDestroyTag(int _tagHandle);
	public Delegate_maNFCDestroyTag maNFCDestroyTag = null;
	public delegate long Delegate_maNFCConnectTag(int _tagHandle);
	public Delegate_maNFCConnectTag maNFCConnectTag = null;
	public delegate long Delegate_maNFCCloseTag(int _tagHandle);
	public Delegate_maNFCCloseTag maNFCCloseTag = null;
	public delegate long Delegate_maNFCIsType(int _tagHandle, int _type);
	public Delegate_maNFCIsType maNFCIsType = null;
	public delegate long Delegate_maNFCGetTypedTag(int _tagHandle, int _type);
	public Delegate_maNFCGetTypedTag maNFCGetTypedTag = null;
	public delegate long Delegate_maNFCBatchStart(int _tagHandle);
	public Delegate_maNFCBatchStart maNFCBatchStart = null;
	public delegate long Delegate_maNFCBatchCommit(int _tagHandle);
	public Delegate_maNFCBatchCommit maNFCBatchCommit = null;
	public delegate long Delegate_maNFCBatchRollback(int _tagHandle);
	public Delegate_maNFCBatchRollback maNFCBatchRollback = null;
	public delegate long Delegate_maNFCTransceive(int _tag, int _src, int _srcLen, int _dst, int _dstLen);
	public Delegate_maNFCTransceive maNFCTransceive = null;
	public delegate long Delegate_maNFCSetReadOnly(int _tag);
	public Delegate_maNFCSetReadOnly maNFCSetReadOnly = null;
	public delegate long Delegate_maNFCIsReadOnly(int _tag);
	public Delegate_maNFCIsReadOnly maNFCIsReadOnly = null;
	public delegate long Delegate_maNFCGetSize(int _tag);
	public Delegate_maNFCGetSize maNFCGetSize = null;
	public delegate long Delegate_maNFCGetId(int _tag, int _dst, int _len);
	public Delegate_maNFCGetId maNFCGetId = null;
	public delegate long Delegate_maNFCReadNDEFMessage(int _tag);
	public Delegate_maNFCReadNDEFMessage maNFCReadNDEFMessage = null;
	public delegate long Delegate_maNFCWriteNDEFMessage(int _tag, int _ndefMessage);
	public Delegate_maNFCWriteNDEFMessage maNFCWriteNDEFMessage = null;
	public delegate long Delegate_maNFCCreateNDEFMessage(int _recordCount);
	public Delegate_maNFCCreateNDEFMessage maNFCCreateNDEFMessage = null;
	public delegate long Delegate_maNFCGetNDEFMessage(int _tag);
	public Delegate_maNFCGetNDEFMessage maNFCGetNDEFMessage = null;
	public delegate long Delegate_maNFCGetNDEFRecord(int _ndefMessage, int _ix);
	public Delegate_maNFCGetNDEFRecord maNFCGetNDEFRecord = null;
	public delegate long Delegate_maNFCGetNDEFRecordCount(int _ndefMessage);
	public Delegate_maNFCGetNDEFRecordCount maNFCGetNDEFRecordCount = null;
	public delegate long Delegate_maNFCGetNDEFId(int _ndefRecord, int _dst, int _len);
	public Delegate_maNFCGetNDEFId maNFCGetNDEFId = null;
	public delegate long Delegate_maNFCGetNDEFPayload(int _ndefRecord, int _dst, int _len);
	public Delegate_maNFCGetNDEFPayload maNFCGetNDEFPayload = null;
	public delegate long Delegate_maNFCGetNDEFTnf(int _ndefRecord);
	public Delegate_maNFCGetNDEFTnf maNFCGetNDEFTnf = null;
	public delegate long Delegate_maNFCGetNDEFType(int _ndefRecord, int _dst, int _len);
	public Delegate_maNFCGetNDEFType maNFCGetNDEFType = null;
	public delegate long Delegate_maNFCSetNDEFId(int _ndefRecord, int _dst, int _len);
	public Delegate_maNFCSetNDEFId maNFCSetNDEFId = null;
	public delegate long Delegate_maNFCSetNDEFPayload(int _ndefRecord, int _dst, int _len);
	public Delegate_maNFCSetNDEFPayload maNFCSetNDEFPayload = null;
	public delegate long Delegate_maNFCSetNDEFTnf(int _ndefRecord, int _tnf);
	public Delegate_maNFCSetNDEFTnf maNFCSetNDEFTnf = null;
	public delegate long Delegate_maNFCSetNDEFType(int _ndefRecord, int _dst, int _len);
	public Delegate_maNFCSetNDEFType maNFCSetNDEFType = null;
	public delegate long Delegate_maNFCAuthenticateMifareSector(int _tagHandle, int _keyType, int _sectorIndex, int _keySrc, int _keyLen);
	public Delegate_maNFCAuthenticateMifareSector maNFCAuthenticateMifareSector = null;
	public delegate long Delegate_maNFCGetMifareSectorCount(int _tagHandle);
	public Delegate_maNFCGetMifareSectorCount maNFCGetMifareSectorCount = null;
	public delegate long Delegate_maNFCGetMifareBlockCountInSector(int _tagHandle, int _sector);
	public Delegate_maNFCGetMifareBlockCountInSector maNFCGetMifareBlockCountInSector = null;
	public delegate long Delegate_maNFCMifareSectorToBlock(int _tagHandle, int _sector);
	public Delegate_maNFCMifareSectorToBlock maNFCMifareSectorToBlock = null;
	public delegate long Delegate_maNFCReadMifareBlocks(int _tagHandle, int _firstBlock, int _dst, int _len);
	public Delegate_maNFCReadMifareBlocks maNFCReadMifareBlocks = null;
	public delegate long Delegate_maNFCWriteMifareBlocks(int _tagHandle, int _firstBlock, int _dst, int _len);
	public Delegate_maNFCWriteMifareBlocks maNFCWriteMifareBlocks = null;
	public delegate long Delegate_maNFCReadMifarePages(int _tagHandle, int _firstPage, int _dst, int _len);
	public Delegate_maNFCReadMifarePages maNFCReadMifarePages = null;
	public delegate long Delegate_maNFCWriteMifarePages(int _tagHandle, int _firstPage, int _dst, int _len);
	public Delegate_maNFCWriteMifarePages maNFCWriteMifarePages = null;
	public delegate long Delegate_maDBOpen(int _path);
	public Delegate_maDBOpen maDBOpen = null;
	public delegate long Delegate_maDBClose(int _databaseHandle);
	public Delegate_maDBClose maDBClose = null;
	public delegate long Delegate_maDBExecSQL(int _databaseHandle, int _sql);
	public Delegate_maDBExecSQL maDBExecSQL = null;
	public delegate long Delegate_maDBExecSQLParams(int _databaseHandle, int _sql, int _params, int _paramCount);
	public Delegate_maDBExecSQLParams maDBExecSQLParams = null;
	public delegate long Delegate_maDBCursorDestroy(int _cursorHandle);
	public Delegate_maDBCursorDestroy maDBCursorDestroy = null;
	public delegate long Delegate_maDBCursorNext(int _cursorHandle);
	public Delegate_maDBCursorNext maDBCursorNext = null;
	public delegate long Delegate_maDBCursorGetColumnData(int _cursorHandle, int _columnIndex, int _placeholder);
	public Delegate_maDBCursorGetColumnData maDBCursorGetColumnData = null;
	public delegate long Delegate_maDBCursorGetColumnText(int _cursorHandle, int _columnIndex, int _buffer, int _bufferSize);
	public Delegate_maDBCursorGetColumnText maDBCursorGetColumnText = null;
	public delegate long Delegate_maDBCursorGetColumnInt(int _cursorHandle, int _columnIndex, int _value);
	public Delegate_maDBCursorGetColumnInt maDBCursorGetColumnInt = null;
	public delegate long Delegate_maDBCursorGetColumnDouble(int _cursorHandle, int _columnIndex, int _value);
	public Delegate_maDBCursorGetColumnDouble maDBCursorGetColumnDouble = null;
	public delegate long Delegate_maAudioDataCreateFromResource(int _mime, int _data, int _offset, int _length, int _flags);
	public Delegate_maAudioDataCreateFromResource maAudioDataCreateFromResource = null;
	public delegate long Delegate_maAudioDataCreateFromURL(int _mime, int _url, int _flags);
	public Delegate_maAudioDataCreateFromURL maAudioDataCreateFromURL = null;
	public delegate long Delegate_maAudioDataDestroy(int _audioData);
	public Delegate_maAudioDataDestroy maAudioDataDestroy = null;
	public delegate long Delegate_maAudioInstanceCreate(int _audioData);
	public Delegate_maAudioInstanceCreate maAudioInstanceCreate = null;
	public delegate long Delegate_maAudioInstanceCreateDynamic(int _sampleRate, int _numChannels, int _bufferSize);
	public Delegate_maAudioInstanceCreateDynamic maAudioInstanceCreateDynamic = null;
	public delegate long Delegate_maAudioSubmitBuffer(int _instance, int _buffer, int _numBytes);
	public Delegate_maAudioSubmitBuffer maAudioSubmitBuffer = null;
	public delegate long Delegate_maAudioGetPendingBufferCount(int _instance);
	public Delegate_maAudioGetPendingBufferCount maAudioGetPendingBufferCount = null;
	public delegate long Delegate_maAudioInstanceDestroy(int _audioInstance);
	public Delegate_maAudioInstanceDestroy maAudioInstanceDestroy = null;
	public delegate long Delegate_maAudioGetLength(int _audio);
	public Delegate_maAudioGetLength maAudioGetLength = null;
	public delegate long Delegate_maAudioSetNumberOfLoops(int _audio, int _loops);
	public Delegate_maAudioSetNumberOfLoops maAudioSetNumberOfLoops = null;
	public delegate long Delegate_maAudioPrepare(int _audio, int _async);
	public Delegate_maAudioPrepare maAudioPrepare = null;
	public delegate long Delegate_maAudioPlay(int _audio);
	public Delegate_maAudioPlay maAudioPlay = null;
	public delegate long Delegate_maAudioSetPosition(int _audio, int _milliseconds);
	public Delegate_maAudioSetPosition maAudioSetPosition = null;
	public delegate long Delegate_maAudioGetPosition(int _audio);
	public Delegate_maAudioGetPosition maAudioGetPosition = null;
	public delegate long Delegate_maAudioSetVolume(int _audio, float _volume);
	public Delegate_maAudioSetVolume maAudioSetVolume = null;
	public delegate long Delegate_maAudioPause(int _audio);
	public Delegate_maAudioPause maAudioPause = null;
	public delegate long Delegate_maAudioStop(int _audio);
	public Delegate_maAudioStop maAudioStop = null;
	public delegate long Delegate_maSyscallPanicsEnable();
	public Delegate_maSyscallPanicsEnable maSyscallPanicsEnable = null;
	public delegate long Delegate_maSyscallPanicsDisable();
	public Delegate_maSyscallPanicsDisable maSyscallPanicsDisable = null;
	public delegate long Delegate_maAdsBannerCreate(int _bannerSize, int _publisherID);
	public Delegate_maAdsBannerCreate maAdsBannerCreate = null;
	public delegate long Delegate_maAdsBannerDestroy(int _bannerHandle);
	public Delegate_maAdsBannerDestroy maAdsBannerDestroy = null;
	public delegate long Delegate_maAdsAddBannerToLayout(int _bannerHandle, int _layoutHandle);
	public Delegate_maAdsAddBannerToLayout maAdsAddBannerToLayout = null;
	public delegate long Delegate_maAdsRemoveBannerFromLayout(int _bannerHandle, int _layoutHandle);
	public Delegate_maAdsRemoveBannerFromLayout maAdsRemoveBannerFromLayout = null;
	public delegate long Delegate_maAdsBannerSetProperty(int _bannerHandle, int _property, int _value);
	public Delegate_maAdsBannerSetProperty maAdsBannerSetProperty = null;
	public delegate long Delegate_maAdsBannerGetProperty(int _bannerHandle, int _property, int _value, int _bufSize);
	public Delegate_maAdsBannerGetProperty maAdsBannerGetProperty = null;
	public delegate long Delegate_maNotificationLocalCreate();
	public Delegate_maNotificationLocalCreate maNotificationLocalCreate = null;
	public delegate long Delegate_maNotificationLocalDestroy(int _notificationHandle);
	public Delegate_maNotificationLocalDestroy maNotificationLocalDestroy = null;
	public delegate long Delegate_maNotificationLocalSetProperty(int _notificationHandle, int _property, int _value);
	public Delegate_maNotificationLocalSetProperty maNotificationLocalSetProperty = null;
	public delegate long Delegate_maNotificationLocalGetProperty(int _notificationHandle, int _property, int _value, int _bufSize);
	public Delegate_maNotificationLocalGetProperty maNotificationLocalGetProperty = null;
	public delegate long Delegate_maNotificationLocalSchedule(int _notificationHandle);
	public Delegate_maNotificationLocalSchedule maNotificationLocalSchedule = null;
	public delegate long Delegate_maNotificationLocalUnschedule(int _notificationHandle);
	public Delegate_maNotificationLocalUnschedule maNotificationLocalUnschedule = null;
	public delegate long Delegate_maNotificationPushRegister(int _pushNotificationTypes, int _senderId);
	public Delegate_maNotificationPushRegister maNotificationPushRegister = null;
	public delegate long Delegate_maNotificationPushUnregister();
	public Delegate_maNotificationPushUnregister maNotificationPushUnregister = null;
	public delegate long Delegate_maNotificationPushGetRegistration(int _registrationMesssage, int _registrationMessageSize);
	public Delegate_maNotificationPushGetRegistration maNotificationPushGetRegistration = null;
	public delegate long Delegate_maNotificationPushGetData(int _pushNotificationHandle, int _pushNotificationData);
	public Delegate_maNotificationPushGetData maNotificationPushGetData = null;
	public delegate long Delegate_maNotificationPushDestroy(int _notificationHandle);
	public Delegate_maNotificationPushDestroy maNotificationPushDestroy = null;
	public delegate long Delegate_maNotificationSetIconBadge(int _applicationIconBadgeNumber);
	public Delegate_maNotificationSetIconBadge maNotificationSetIconBadge = null;
	public delegate long Delegate_maNotificationGetIconBadge();
	public Delegate_maNotificationGetIconBadge maNotificationGetIconBadge = null;
	public delegate long Delegate_maNotificationPushSetTickerText(int _tickerText);
	public Delegate_maNotificationPushSetTickerText maNotificationPushSetTickerText = null;
	public delegate long Delegate_maNotificationPushSetMessageTitle(int _messageTitle);
	public Delegate_maNotificationPushSetMessageTitle maNotificationPushSetMessageTitle = null;
	public delegate long Delegate_maNotificationPushSetDisplayFlag(int _displayFlag);
	public Delegate_maNotificationPushSetDisplayFlag maNotificationPushSetDisplayFlag = null;
	public delegate long Delegate_maCaptureSetProperty(int _property, int _value);
	public Delegate_maCaptureSetProperty maCaptureSetProperty = null;
	public delegate long Delegate_maCaptureGetProperty(int _property, int _value, int _bufSize);
	public Delegate_maCaptureGetProperty maCaptureGetProperty = null;
	public delegate long Delegate_maCaptureAction(int _action);
	public Delegate_maCaptureAction maCaptureAction = null;
	public delegate long Delegate_maCaptureWriteImage(int _handle, int _fullPath, int _fullPathBufSize);
	public Delegate_maCaptureWriteImage maCaptureWriteImage = null;
	public delegate long Delegate_maCaptureGetImagePath(int _handle, int _buffer, int _bufferSize);
	public Delegate_maCaptureGetImagePath maCaptureGetImagePath = null;
	public delegate long Delegate_maCaptureGetVideoPath(int _handle, int _buffer, int _bufferSize);
	public Delegate_maCaptureGetVideoPath maCaptureGetVideoPath = null;
	public delegate long Delegate_maCaptureDestroyData(int _handle);
	public Delegate_maCaptureDestroyData maCaptureDestroyData = null;
	public delegate long Delegate_maPurchaseSupported();
	public Delegate_maPurchaseSupported maPurchaseSupported = null;
	public delegate long Delegate_maPurchaseCreate(int _productHandle, int _productID);
	public Delegate_maPurchaseCreate maPurchaseCreate = null;
	public delegate long Delegate_maPurchaseSetPublicKey(int _developerPublicKey);
	public Delegate_maPurchaseSetPublicKey maPurchaseSetPublicKey = null;
	public delegate long Delegate_maPurchaseRequest(int _productHandle, int _quantity);
	public Delegate_maPurchaseRequest maPurchaseRequest = null;
	public delegate long Delegate_maPurchaseVerifyReceipt(int _productHandle);
	public Delegate_maPurchaseVerifyReceipt maPurchaseVerifyReceipt = null;
	public delegate long Delegate_maPurchaseGetField(int _productHandle, int _property, int _buffer, int _bufferSize);
	public Delegate_maPurchaseGetField maPurchaseGetField = null;
	public delegate long Delegate_maPurchaseSetStoreURL(int _url);
	public Delegate_maPurchaseSetStoreURL maPurchaseSetStoreURL = null;
	public delegate long Delegate_maPurchaseGetName(int _productHandle, int _buffer, int _bufferSize);
	public Delegate_maPurchaseGetName maPurchaseGetName = null;
	public delegate long Delegate_maPurchaseRestoreTransactions();
	public Delegate_maPurchaseRestoreTransactions maPurchaseRestoreTransactions = null;
	public delegate long Delegate_maPurchaseDestroy(int _productHandle);
	public Delegate_maPurchaseDestroy maPurchaseDestroy = null;
}

public class IoctlInvoker {

	private Core mCore;
	private Ioctls mIoctls;

	public IoctlInvoker(Core core, Ioctls ioctls) {
		mCore = core;
		mIoctls = ioctls;
	}

	public long InvokeIoctl(int id, int a, int b, int c) {
		long result;
		switch(id) {
			case 1:
			if(mIoctls.maExtensionModuleLoad == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maExtensionModuleLoad(a, b);
#if SYSCALL_LOG
			Util.Log("maExtensionModuleLoad("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 2:
			if(mIoctls.maExtensionFunctionLoad == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maExtensionFunctionLoad(a, b);
#if SYSCALL_LOG
			Util.Log("maExtensionFunctionLoad("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 3:
			if(mIoctls.maFontLoadDefault == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFontLoadDefault(a, b, c);
#if SYSCALL_LOG
			Util.Log("maFontLoadDefault("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 4:
			if(mIoctls.maFontSetCurrent == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFontSetCurrent(a);
#if SYSCALL_LOG
			Util.Log("maFontSetCurrent("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 5:
			if(mIoctls.maFontGetCount == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFontGetCount();
#if SYSCALL_LOG
			Util.Log("maFontGetCount("+
				"): "+result+"\n");
#endif
		return result;
			case 6:
			if(mIoctls.maFontGetName == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFontGetName(a, b, c);
#if SYSCALL_LOG
			Util.Log("maFontGetName("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 7:
			if(mIoctls.maFontLoadWithName == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFontLoadWithName(a, b);
#if SYSCALL_LOG
			Util.Log("maFontLoadWithName("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 8:
			if(mIoctls.maFontDelete == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFontDelete(a);
#if SYSCALL_LOG
			Util.Log("maFontDelete("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 9:
			if(mIoctls.maReportCallStack == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maReportCallStack();
#if SYSCALL_LOG
			Util.Log("maReportCallStack("+
				"): "+result+"\n");
#endif
		return result;
			case 10:
			if(mIoctls.maDumpCallStackEx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDumpCallStackEx(a, b);
#if SYSCALL_LOG
			Util.Log("maDumpCallStackEx("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 11:
			if(mIoctls.maProtectMemory == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maProtectMemory(a, b);
#if SYSCALL_LOG
			Util.Log("maProtectMemory("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 12:
			if(mIoctls.maUnprotectMemory == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maUnprotectMemory(a, b);
#if SYSCALL_LOG
			Util.Log("maUnprotectMemory("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 13:
			if(mIoctls.maSetMemoryProtection == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSetMemoryProtection(a);
#if SYSCALL_LOG
			Util.Log("maSetMemoryProtection("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 14:
			if(mIoctls.maGetMemoryProtection == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maGetMemoryProtection();
#if SYSCALL_LOG
			Util.Log("maGetMemoryProtection("+
				"): "+result+"\n");
#endif
		return result;
			case 15:
			if(mIoctls.maGetBatteryCharge == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maGetBatteryCharge();
#if SYSCALL_LOG
			Util.Log("maGetBatteryCharge("+
				"): "+result+"\n");
#endif
		return result;
			case 16:
			if(mIoctls.maStartSpeaking == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStartSpeaking(a);
#if SYSCALL_LOG
			Util.Log("maStartSpeaking("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 17:
			if(mIoctls.maStopSpeaking == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStopSpeaking();
#if SYSCALL_LOG
			Util.Log("maStopSpeaking("+
				"): "+result+"\n");
#endif
		return result;
			case 18:
			if(mIoctls.maIsSpeaking == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maIsSpeaking();
#if SYSCALL_LOG
			Util.Log("maIsSpeaking("+
				"): "+result+"\n");
#endif
		return result;
			case 19:
			if(mIoctls.maSetSpeechVolume == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSetSpeechVolume(a);
#if SYSCALL_LOG
			Util.Log("maSetSpeechVolume("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 20:
			if(mIoctls.maSetSpeechSpeed == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSetSpeechSpeed(a);
#if SYSCALL_LOG
			Util.Log("maSetSpeechSpeed("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 21:
			if(mIoctls.maSetSpeechPitch == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSetSpeechPitch(a);
#if SYSCALL_LOG
			Util.Log("maSetSpeechPitch("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 22:
			if(mIoctls.sinh == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.sinh(MoSync.Util.ConvertToDouble(a, b));
#if SYSCALL_LOG
			Util.Log("sinh("+
				MoSync.Util.ConvertToDouble(a, b)+
				"): "+result+"\n");
#endif
		return result;
			case 23:
			if(mIoctls.cosh == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.cosh(MoSync.Util.ConvertToDouble(a, b));
#if SYSCALL_LOG
			Util.Log("cosh("+
				MoSync.Util.ConvertToDouble(a, b)+
				"): "+result+"\n");
#endif
		return result;
			case 24:
			if(mIoctls.atanh == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.atanh(MoSync.Util.ConvertToDouble(a, b));
#if SYSCALL_LOG
			Util.Log("atanh("+
				MoSync.Util.ConvertToDouble(a, b)+
				"): "+result+"\n");
#endif
		return result;
			case 25:
			if(mIoctls.maLockKeypad == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maLockKeypad();
#if SYSCALL_LOG
			Util.Log("maLockKeypad("+
				"): "+result+"\n");
#endif
		return result;
			case 26:
			if(mIoctls.maUnlockKeypad == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maUnlockKeypad();
#if SYSCALL_LOG
			Util.Log("maUnlockKeypad("+
				"): "+result+"\n");
#endif
		return result;
			case 27:
			if(mIoctls.maKeypadIsLocked == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maKeypadIsLocked();
#if SYSCALL_LOG
			Util.Log("maKeypadIsLocked("+
				"): "+result+"\n");
#endif
		return result;
			case 28:
			if(mIoctls.maWriteLog == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWriteLog(a, b);
		return result;
			case 29:
			if(mIoctls.maBtStartDeviceDiscovery == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maBtStartDeviceDiscovery(a);
#if SYSCALL_LOG
			Util.Log("maBtStartDeviceDiscovery("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 30:
			if(mIoctls.maBtGetNewDevice == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maBtGetNewDevice(a);
#if SYSCALL_LOG
			Util.Log("maBtGetNewDevice("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 31:
			if(mIoctls.maBtStartServiceDiscovery == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maBtStartServiceDiscovery(a, b);
#if SYSCALL_LOG
			Util.Log("maBtStartServiceDiscovery("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 32:
			if(mIoctls.maBtGetNextServiceSize == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maBtGetNextServiceSize(a);
#if SYSCALL_LOG
			Util.Log("maBtGetNextServiceSize("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 33:
			if(mIoctls.maBtGetNewService == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maBtGetNewService(a);
#if SYSCALL_LOG
			Util.Log("maBtGetNewService("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 34:
			if(mIoctls.maBtCancelDiscovery == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maBtCancelDiscovery();
#if SYSCALL_LOG
			Util.Log("maBtCancelDiscovery("+
				"): "+result+"\n");
#endif
		return result;
			case 35:
			if(mIoctls.maWlanStartDiscovery == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWlanStartDiscovery();
#if SYSCALL_LOG
			Util.Log("maWlanStartDiscovery("+
				"): "+result+"\n");
#endif
		return result;
			case 36:
			if(mIoctls.maWlanGetNewAp == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWlanGetNewAp(a);
#if SYSCALL_LOG
			Util.Log("maWlanGetNewAp("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 37:
			if(mIoctls.maLocationStart == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maLocationStart();
#if SYSCALL_LOG
			Util.Log("maLocationStart("+
				"): "+result+"\n");
#endif
		return result;
			case 38:
			if(mIoctls.maLocationStop == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maLocationStop();
#if SYSCALL_LOG
			Util.Log("maLocationStop("+
				"): "+result+"\n");
#endif
		return result;
			case 39:
			if(mIoctls.maPlatformRequest == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPlatformRequest(a);
#if SYSCALL_LOG
			Util.Log("maPlatformRequest("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 40:
			if(mIoctls.maFileOpen == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileOpen(a, b);
#if SYSCALL_LOG
			Util.Log("maFileOpen("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 41:
			if(mIoctls.maFileExists == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileExists(a);
#if SYSCALL_LOG
			Util.Log("maFileExists("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 42:
			if(mIoctls.maFileClose == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileClose(a);
#if SYSCALL_LOG
			Util.Log("maFileClose("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 43:
			if(mIoctls.maFileCreate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileCreate(a);
#if SYSCALL_LOG
			Util.Log("maFileCreate("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 44:
			if(mIoctls.maFileDelete == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileDelete(a);
#if SYSCALL_LOG
			Util.Log("maFileDelete("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 45:
			if(mIoctls.maFileSize == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileSize(a);
#if SYSCALL_LOG
			Util.Log("maFileSize("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 46:
			if(mIoctls.maFileAvailableSpace == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileAvailableSpace(a);
#if SYSCALL_LOG
			Util.Log("maFileAvailableSpace("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 47:
			if(mIoctls.maFileTotalSpace == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileTotalSpace(a);
#if SYSCALL_LOG
			Util.Log("maFileTotalSpace("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 48:
			if(mIoctls.maFileDate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileDate(a);
#if SYSCALL_LOG
			Util.Log("maFileDate("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 49:
			if(mIoctls.maFileRename == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileRename(a, b);
#if SYSCALL_LOG
			Util.Log("maFileRename("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 50:
			if(mIoctls.maFileTruncate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileTruncate(a, b);
#if SYSCALL_LOG
			Util.Log("maFileTruncate("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 51:
			if(mIoctls.maFileWrite == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileWrite(a, b, c);
#if SYSCALL_LOG
			Util.Log("maFileWrite("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 52:
			if(mIoctls.maFileWriteFromData == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileWriteFromData(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maFileWriteFromData("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 53:
			if(mIoctls.maFileRead == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileRead(a, b, c);
#if SYSCALL_LOG
			Util.Log("maFileRead("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 54:
			if(mIoctls.maFileReadToData == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileReadToData(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maFileReadToData("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 55:
			if(mIoctls.maFileTell == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileTell(a);
#if SYSCALL_LOG
			Util.Log("maFileTell("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 56:
			if(mIoctls.maFileSeek == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileSeek(a, b, c);
#if SYSCALL_LOG
			Util.Log("maFileSeek("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 57:
			if(mIoctls.maFileListStart == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileListStart(a, b, c);
#if SYSCALL_LOG
			Util.Log("maFileListStart("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 58:
			if(mIoctls.maFileListNext == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileListNext(a, b, c);
#if SYSCALL_LOG
			Util.Log("maFileListNext("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 59:
			if(mIoctls.maFileListClose == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFileListClose(a);
#if SYSCALL_LOG
			Util.Log("maFileListClose("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 60:
			if(mIoctls.maSendTextSMS == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSendTextSMS(a, b);
#if SYSCALL_LOG
			Util.Log("maSendTextSMS("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 61:
			if(mIoctls.maRecordSetup == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maRecordSetup();
#if SYSCALL_LOG
			Util.Log("maRecordSetup("+
				"): "+result+"\n");
#endif
		return result;
			case 62:
			if(mIoctls.maRecordStart == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maRecordStart(a);
#if SYSCALL_LOG
			Util.Log("maRecordStart("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 63:
			if(mIoctls.maRecordPause == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maRecordPause(a);
#if SYSCALL_LOG
			Util.Log("maRecordPause("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 64:
			if(mIoctls.maGetCellInfo == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maGetCellInfo(a);
#if SYSCALL_LOG
			Util.Log("maGetCellInfo("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 65:
			if(mIoctls.maCallDial == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCallDial(a);
#if SYSCALL_LOG
			Util.Log("maCallDial("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 66:
			if(mIoctls.maCallAnswer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCallAnswer();
#if SYSCALL_LOG
			Util.Log("maCallAnswer("+
				"): "+result+"\n");
#endif
		return result;
			case 67:
			if(mIoctls.maCallHangup == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCallHangup();
#if SYSCALL_LOG
			Util.Log("maCallHangup("+
				"): "+result+"\n");
#endif
		return result;
			case 68:
			if(mIoctls.maSoundPlayConn == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSoundPlayConn(a, b);
#if SYSCALL_LOG
			Util.Log("maSoundPlayConn("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 69:
			if(mIoctls.maStreamVideoStart == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStreamVideoStart(a);
#if SYSCALL_LOG
			Util.Log("maStreamVideoStart("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 70:
			if(mIoctls.maStreamVideoSize == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStreamVideoSize(a);
#if SYSCALL_LOG
			Util.Log("maStreamVideoSize("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 71:
			if(mIoctls.maStreamVideoSetFrame == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStreamVideoSetFrame(a, b);
#if SYSCALL_LOG
			Util.Log("maStreamVideoSetFrame("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 72:
			if(mIoctls.maStreamLength == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStreamLength(a);
#if SYSCALL_LOG
			Util.Log("maStreamLength("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 73:
			if(mIoctls.maStreamPos == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStreamPos(a);
#if SYSCALL_LOG
			Util.Log("maStreamPos("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 74:
			if(mIoctls.maStreamSetPos == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStreamSetPos(a, b);
#if SYSCALL_LOG
			Util.Log("maStreamSetPos("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 75:
			if(mIoctls.maStreamPause == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStreamPause(a);
#if SYSCALL_LOG
			Util.Log("maStreamPause("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 76:
			if(mIoctls.maStreamResume == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStreamResume(a);
#if SYSCALL_LOG
			Util.Log("maStreamResume("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 77:
			if(mIoctls.maStreamClose == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maStreamClose(a);
#if SYSCALL_LOG
			Util.Log("maStreamClose("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 78:
			if(mIoctls.maFrameBufferGetInfo == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFrameBufferGetInfo(a);
#if SYSCALL_LOG
			Util.Log("maFrameBufferGetInfo("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 79:
			if(mIoctls.maFrameBufferInit == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFrameBufferInit(a);
#if SYSCALL_LOG
			Util.Log("maFrameBufferInit("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 80:
			if(mIoctls.maFrameBufferClose == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maFrameBufferClose();
#if SYSCALL_LOG
			Util.Log("maFrameBufferClose("+
				"): "+result+"\n");
#endif
		return result;
			case 81:
			if(mIoctls.maAudioBufferInit == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioBufferInit(a);
#if SYSCALL_LOG
			Util.Log("maAudioBufferInit("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 82:
			if(mIoctls.maAudioBufferReady == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioBufferReady();
#if SYSCALL_LOG
			Util.Log("maAudioBufferReady("+
				"): "+result+"\n");
#endif
		return result;
			case 83:
			if(mIoctls.maAudioBufferClose == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioBufferClose();
#if SYSCALL_LOG
			Util.Log("maAudioBufferClose("+
				"): "+result+"\n");
#endif
		return result;
			case 84:
			if(mIoctls.maAccept == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAccept(a);
#if SYSCALL_LOG
			Util.Log("maAccept("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 85:
			if(mIoctls.maSendToBackground == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSendToBackground();
#if SYSCALL_LOG
			Util.Log("maSendToBackground("+
				"): "+result+"\n");
#endif
		return result;
			case 86:
			if(mIoctls.maBringToForeground == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maBringToForeground();
#if SYSCALL_LOG
			Util.Log("maBringToForeground("+
				"): "+result+"\n");
#endif
		return result;
			case 87:
			if(mIoctls.maAddDataSize == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAddDataSize(a, b);
#if SYSCALL_LOG
			Util.Log("maAddDataSize("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 88:
			if(mIoctls.maGetSystemProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maGetSystemProperty(a, b, c);
#if SYSCALL_LOG
			Util.Log("maGetSystemProperty("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 89:
			if(mIoctls.maIapSave == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maIapSave();
#if SYSCALL_LOG
			Util.Log("maIapSave("+
				"): "+result+"\n");
#endif
		return result;
			case 90:
			if(mIoctls.maIapReset == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maIapReset();
#if SYSCALL_LOG
			Util.Log("maIapReset("+
				"): "+result+"\n");
#endif
		return result;
			case 91:
			if(mIoctls.maIapShutdown == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maIapShutdown();
#if SYSCALL_LOG
			Util.Log("maIapShutdown("+
				"): "+result+"\n");
#endif
		return result;
			case 92:
			if(mIoctls.maNetworkStatus == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNetworkStatus();
#if SYSCALL_LOG
			Util.Log("maNetworkStatus("+
				"): "+result+"\n");
#endif
		return result;
			case 93:
			if(mIoctls.maIapSetMethod == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maIapSetMethod(a);
#if SYSCALL_LOG
			Util.Log("maIapSetMethod("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 94:
			if(mIoctls.maIapSetFilter == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maIapSetFilter(a);
#if SYSCALL_LOG
			Util.Log("maIapSetFilter("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 95:
			if(mIoctls.maPimListOpen == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimListOpen(a);
#if SYSCALL_LOG
			Util.Log("maPimListOpen("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 96:
			if(mIoctls.maPimListNext == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimListNext(a);
#if SYSCALL_LOG
			Util.Log("maPimListNext("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 97:
			if(mIoctls.maPimListClose == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimListClose(a);
#if SYSCALL_LOG
			Util.Log("maPimListClose("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 98:
			if(mIoctls.maPimItemCount == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemCount(a);
#if SYSCALL_LOG
			Util.Log("maPimItemCount("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 99:
			if(mIoctls.maPimItemGetField == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemGetField(a, b);
#if SYSCALL_LOG
			Util.Log("maPimItemGetField("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 100:
			if(mIoctls.maPimItemFieldCount == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemFieldCount(a, b);
#if SYSCALL_LOG
			Util.Log("maPimItemFieldCount("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 101:
			if(mIoctls.maPimItemGetAttributes == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemGetAttributes(a, b, c);
#if SYSCALL_LOG
			Util.Log("maPimItemGetAttributes("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 102:
			if(mIoctls.maPimItemSetLabel == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemSetLabel(a, b);
#if SYSCALL_LOG
			Util.Log("maPimItemSetLabel("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 103:
			if(mIoctls.maPimItemGetLabel == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemGetLabel(a, b);
#if SYSCALL_LOG
			Util.Log("maPimItemGetLabel("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 104:
			if(mIoctls.maPimFieldType == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimFieldType(a, b);
#if SYSCALL_LOG
			Util.Log("maPimFieldType("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 105:
			if(mIoctls.maPimItemGetValue == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemGetValue(a, b);
#if SYSCALL_LOG
			Util.Log("maPimItemGetValue("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 106:
			if(mIoctls.maPimItemSetValue == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemSetValue(a, b, c);
#if SYSCALL_LOG
			Util.Log("maPimItemSetValue("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 107:
			if(mIoctls.maPimItemAddValue == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemAddValue(a, b);
#if SYSCALL_LOG
			Util.Log("maPimItemAddValue("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 108:
			if(mIoctls.maPimItemRemoveValue == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemRemoveValue(a, b, c);
#if SYSCALL_LOG
			Util.Log("maPimItemRemoveValue("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 109:
			if(mIoctls.maPimItemClose == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemClose(a);
#if SYSCALL_LOG
			Util.Log("maPimItemClose("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 110:
			if(mIoctls.maPimItemCreate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemCreate(a);
#if SYSCALL_LOG
			Util.Log("maPimItemCreate("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 111:
			if(mIoctls.maPimItemRemove == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPimItemRemove(a, b);
#if SYSCALL_LOG
			Util.Log("maPimItemRemove("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 112:
			if(mIoctls.maWidgetCreate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetCreate(a);
#if SYSCALL_LOG
			Util.Log("maWidgetCreate("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 113:
			if(mIoctls.maWidgetDestroy == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetDestroy(a);
#if SYSCALL_LOG
			Util.Log("maWidgetDestroy("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 114:
			if(mIoctls.maWidgetAddChild == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetAddChild(a, b);
#if SYSCALL_LOG
			Util.Log("maWidgetAddChild("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 115:
			if(mIoctls.maWidgetInsertChild == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetInsertChild(a, b, c);
#if SYSCALL_LOG
			Util.Log("maWidgetInsertChild("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 116:
			if(mIoctls.maWidgetRemoveChild == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetRemoveChild(a);
#if SYSCALL_LOG
			Util.Log("maWidgetRemoveChild("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 117:
			if(mIoctls.maWidgetModalDialogShow == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetModalDialogShow(a);
#if SYSCALL_LOG
			Util.Log("maWidgetModalDialogShow("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 118:
			if(mIoctls.maWidgetModalDialogHide == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetModalDialogHide(a);
#if SYSCALL_LOG
			Util.Log("maWidgetModalDialogHide("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 119:
			if(mIoctls.maWidgetScreenShow == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetScreenShow(a);
#if SYSCALL_LOG
			Util.Log("maWidgetScreenShow("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 120:
			if(mIoctls.maWidgetStackScreenPush == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetStackScreenPush(a, b);
#if SYSCALL_LOG
			Util.Log("maWidgetStackScreenPush("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 121:
			if(mIoctls.maWidgetStackScreenPop == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetStackScreenPop(a);
#if SYSCALL_LOG
			Util.Log("maWidgetStackScreenPop("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 122:
			if(mIoctls.maWidgetSetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetSetProperty(a, b, c);
#if SYSCALL_LOG
			Util.Log("maWidgetSetProperty("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 123:
			if(mIoctls.maWidgetGetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetGetProperty(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maWidgetGetProperty("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 124:
			if(mIoctls.maWidgetScreenAddOptionsMenuItem == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWidgetScreenAddOptionsMenuItem(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maWidgetScreenAddOptionsMenuItem("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 125:
			if(mIoctls.maCameraFormatNumber == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraFormatNumber();
#if SYSCALL_LOG
			Util.Log("maCameraFormatNumber("+
				"): "+result+"\n");
#endif
		return result;
			case 126:
			if(mIoctls.maCameraFormat == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraFormat(a, b);
#if SYSCALL_LOG
			Util.Log("maCameraFormat("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 127:
			if(mIoctls.maCameraStart == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraStart();
#if SYSCALL_LOG
			Util.Log("maCameraStart("+
				"): "+result+"\n");
#endif
		return result;
			case 128:
			if(mIoctls.maCameraStop == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraStop();
#if SYSCALL_LOG
			Util.Log("maCameraStop("+
				"): "+result+"\n");
#endif
		return result;
			case 129:
			if(mIoctls.maCameraSetPreview == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraSetPreview(a);
#if SYSCALL_LOG
			Util.Log("maCameraSetPreview("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 130:
			if(mIoctls.maCameraSelect == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraSelect(a);
#if SYSCALL_LOG
			Util.Log("maCameraSelect("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 131:
			if(mIoctls.maCameraNumber == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraNumber();
#if SYSCALL_LOG
			Util.Log("maCameraNumber("+
				"): "+result+"\n");
#endif
		return result;
			case 132:
			if(mIoctls.maCameraSnapshot == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraSnapshot(a, b);
#if SYSCALL_LOG
			Util.Log("maCameraSnapshot("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 133:
			if(mIoctls.maCameraRecord == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraRecord(a);
#if SYSCALL_LOG
			Util.Log("maCameraRecord("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 134:
			if(mIoctls.maCameraSetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraSetProperty(a, b);
#if SYSCALL_LOG
			Util.Log("maCameraSetProperty("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 135:
			if(mIoctls.maCameraGetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraGetProperty(a, b, c);
#if SYSCALL_LOG
			Util.Log("maCameraGetProperty("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 136:
			if(mIoctls.maCameraPreviewSize == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraPreviewSize();
#if SYSCALL_LOG
			Util.Log("maCameraPreviewSize("+
				"): "+result+"\n");
#endif
		return result;
			case 137:
			if(mIoctls.maCameraPreviewEventEnable == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraPreviewEventEnable(a, b, c);
#if SYSCALL_LOG
			Util.Log("maCameraPreviewEventEnable("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 138:
			if(mIoctls.maCameraPreviewEventDisable == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraPreviewEventDisable();
#if SYSCALL_LOG
			Util.Log("maCameraPreviewEventDisable("+
				"): "+result+"\n");
#endif
		return result;
			case 139:
			if(mIoctls.maCameraPreviewEventConsumed == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCameraPreviewEventConsumed();
#if SYSCALL_LOG
			Util.Log("maCameraPreviewEventConsumed("+
				"): "+result+"\n");
#endif
		return result;
			case 140:
			if(mIoctls.maShowVirtualKeyboard == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maShowVirtualKeyboard();
#if SYSCALL_LOG
			Util.Log("maShowVirtualKeyboard("+
				"): "+result+"\n");
#endif
		return result;
			case 141:
			if(mIoctls.maTextBox == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maTextBox(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("maTextBox("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 142:
			if(mIoctls.maKeyCaptureStart == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maKeyCaptureStart(a);
#if SYSCALL_LOG
			Util.Log("maKeyCaptureStart("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 143:
			if(mIoctls.maKeyCaptureStop == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maKeyCaptureStop();
#if SYSCALL_LOG
			Util.Log("maKeyCaptureStop("+
				"): "+result+"\n");
#endif
		return result;
			case 144:
			if(mIoctls.maHomeScreenEventsOn == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maHomeScreenEventsOn();
#if SYSCALL_LOG
			Util.Log("maHomeScreenEventsOn("+
				"): "+result+"\n");
#endif
		return result;
			case 145:
			if(mIoctls.maHomeScreenEventsOff == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maHomeScreenEventsOff();
#if SYSCALL_LOG
			Util.Log("maHomeScreenEventsOff("+
				"): "+result+"\n");
#endif
		return result;
			case 146:
			if(mIoctls.maHomeScreenShortcutAdd == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maHomeScreenShortcutAdd(a);
#if SYSCALL_LOG
			Util.Log("maHomeScreenShortcutAdd("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 147:
			if(mIoctls.maHomeScreenShortcutRemove == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maHomeScreenShortcutRemove(a);
#if SYSCALL_LOG
			Util.Log("maHomeScreenShortcutRemove("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 148:
			if(mIoctls.maNotificationAdd == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationAdd(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maNotificationAdd("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 149:
			if(mIoctls.maNotificationRemove == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationRemove(a);
#if SYSCALL_LOG
			Util.Log("maNotificationRemove("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 150:
			if(mIoctls.maScreenSetOrientation == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maScreenSetOrientation(a);
#if SYSCALL_LOG
			Util.Log("maScreenSetOrientation("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 151:
			if(mIoctls.maScreenSetFullscreen == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maScreenSetFullscreen(a);
#if SYSCALL_LOG
			Util.Log("maScreenSetFullscreen("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 152:
			if(mIoctls.maAutostartOn == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAutostartOn();
#if SYSCALL_LOG
			Util.Log("maAutostartOn("+
				"): "+result+"\n");
#endif
		return result;
			case 153:
			if(mIoctls.maAutostartOff == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAutostartOff();
#if SYSCALL_LOG
			Util.Log("maAutostartOff("+
				"): "+result+"\n");
#endif
		return result;
			case 154:
			if(mIoctls.maScreenSetSupportedOrientations == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maScreenSetSupportedOrientations(a);
#if SYSCALL_LOG
			Util.Log("maScreenSetSupportedOrientations("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 155:
			if(mIoctls.maScreenGetSupportedOrientations == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maScreenGetSupportedOrientations();
#if SYSCALL_LOG
			Util.Log("maScreenGetSupportedOrientations("+
				"): "+result+"\n");
#endif
		return result;
			case 156:
			if(mIoctls.maScreenGetCurrentOrientation == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maScreenGetCurrentOrientation();
#if SYSCALL_LOG
			Util.Log("maScreenGetCurrentOrientation("+
				"): "+result+"\n");
#endif
		return result;
			case 157:
			if(mIoctls.maScreenStateEventsOn == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maScreenStateEventsOn();
#if SYSCALL_LOG
			Util.Log("maScreenStateEventsOn("+
				"): "+result+"\n");
#endif
		return result;
			case 158:
			if(mIoctls.maScreenStateEventsOff == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maScreenStateEventsOff();
#if SYSCALL_LOG
			Util.Log("maScreenStateEventsOff("+
				"): "+result+"\n");
#endif
		return result;
			case 159:
			if(mIoctls.maWakeLock == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maWakeLock(a);
#if SYSCALL_LOG
			Util.Log("maWakeLock("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 160:
			if(mIoctls.maOpenGLInitFullscreen == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maOpenGLInitFullscreen(a);
#if SYSCALL_LOG
			Util.Log("maOpenGLInitFullscreen("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 161:
			if(mIoctls.maOpenGLCloseFullscreen == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maOpenGLCloseFullscreen();
#if SYSCALL_LOG
			Util.Log("maOpenGLCloseFullscreen("+
				"): "+result+"\n");
#endif
		return result;
			case 162:
			if(mIoctls.maOpenGLTexImage2D == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maOpenGLTexImage2D(a);
#if SYSCALL_LOG
			Util.Log("maOpenGLTexImage2D("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 163:
			if(mIoctls.maOpenGLTexSubImage2D == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maOpenGLTexSubImage2D(a);
#if SYSCALL_LOG
			Util.Log("maOpenGLTexSubImage2D("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 164:
			if(mIoctls.glActiveTexture == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glActiveTexture(a);
#if SYSCALL_LOG
			Util.Log("glActiveTexture("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 165:
			if(mIoctls.glBindBuffer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBindBuffer(a, b);
#if SYSCALL_LOG
			Util.Log("glBindBuffer("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 166:
			if(mIoctls.glBindTexture == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBindTexture(a, b);
#if SYSCALL_LOG
			Util.Log("glBindTexture("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 167:
			if(mIoctls.glBlendFunc == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBlendFunc(a, b);
#if SYSCALL_LOG
			Util.Log("glBlendFunc("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 168:
			if(mIoctls.glBufferData == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBufferData(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glBufferData("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 169:
			if(mIoctls.glBufferSubData == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBufferSubData(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glBufferSubData("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 170:
			if(mIoctls.glClear == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glClear(a);
#if SYSCALL_LOG
			Util.Log("glClear("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 171:
			if(mIoctls.glClearColor == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glClearColor(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)));
#if SYSCALL_LOG
			Util.Log("glClearColor("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				"): "+result+"\n");
#endif
		return result;
			case 172:
			if(mIoctls.glClearDepthf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glClearDepthf(MoSync.Util.ConvertToFloat(a));
#if SYSCALL_LOG
			Util.Log("glClearDepthf("+
				MoSync.Util.ConvertToFloat(a)+
				"): "+result+"\n");
#endif
		return result;
			case 173:
			if(mIoctls.glClearStencil == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glClearStencil(a);
#if SYSCALL_LOG
			Util.Log("glClearStencil("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 174:
			if(mIoctls.glColorMask == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glColorMask(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glColorMask("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 175:
			if(mIoctls.glCompressedTexImage2D == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCompressedTexImage2D(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8), mCore.GetStackValue(12), mCore.GetStackValue(16));
#if SYSCALL_LOG
			Util.Log("glCompressedTexImage2D("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				","+mCore.GetStackValue(12)+
				","+mCore.GetStackValue(16)+
				"): "+result+"\n");
#endif
		return result;
			case 176:
			if(mIoctls.glCompressedTexSubImage2D == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCompressedTexSubImage2D(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8), mCore.GetStackValue(12), mCore.GetStackValue(16), mCore.GetStackValue(20));
#if SYSCALL_LOG
			Util.Log("glCompressedTexSubImage2D("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				","+mCore.GetStackValue(12)+
				","+mCore.GetStackValue(16)+
				","+mCore.GetStackValue(20)+
				"): "+result+"\n");
#endif
		return result;
			case 177:
			if(mIoctls.glCopyTexImage2D == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCopyTexImage2D(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8), mCore.GetStackValue(12), mCore.GetStackValue(16));
#if SYSCALL_LOG
			Util.Log("glCopyTexImage2D("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				","+mCore.GetStackValue(12)+
				","+mCore.GetStackValue(16)+
				"): "+result+"\n");
#endif
		return result;
			case 178:
			if(mIoctls.glCopyTexSubImage2D == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCopyTexSubImage2D(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8), mCore.GetStackValue(12), mCore.GetStackValue(16));
#if SYSCALL_LOG
			Util.Log("glCopyTexSubImage2D("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				","+mCore.GetStackValue(12)+
				","+mCore.GetStackValue(16)+
				"): "+result+"\n");
#endif
		return result;
			case 179:
			if(mIoctls.glCullFace == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCullFace(a);
#if SYSCALL_LOG
			Util.Log("glCullFace("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 180:
			if(mIoctls.glDeleteBuffers == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDeleteBuffers(a, b);
#if SYSCALL_LOG
			Util.Log("glDeleteBuffers("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 181:
			if(mIoctls.glDeleteTextures == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDeleteTextures(a, b);
#if SYSCALL_LOG
			Util.Log("glDeleteTextures("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 182:
			if(mIoctls.glDepthFunc == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDepthFunc(a);
#if SYSCALL_LOG
			Util.Log("glDepthFunc("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 183:
			if(mIoctls.glDepthMask == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDepthMask(a);
#if SYSCALL_LOG
			Util.Log("glDepthMask("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 184:
			if(mIoctls.glDepthRangef == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDepthRangef(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b));
#if SYSCALL_LOG
			Util.Log("glDepthRangef("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				"): "+result+"\n");
#endif
		return result;
			case 185:
			if(mIoctls.glDisable == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDisable(a);
#if SYSCALL_LOG
			Util.Log("glDisable("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 186:
			if(mIoctls.glDrawArrays == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDrawArrays(a, b, c);
#if SYSCALL_LOG
			Util.Log("glDrawArrays("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 187:
			if(mIoctls.glDrawElements == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDrawElements(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glDrawElements("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 188:
			if(mIoctls.glEnable == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glEnable(a);
#if SYSCALL_LOG
			Util.Log("glEnable("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 189:
			if(mIoctls.glFinish == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFinish();
#if SYSCALL_LOG
			Util.Log("glFinish("+
				"): "+result+"\n");
#endif
		return result;
			case 190:
			if(mIoctls.glFlush == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFlush();
#if SYSCALL_LOG
			Util.Log("glFlush("+
				"): "+result+"\n");
#endif
		return result;
			case 191:
			if(mIoctls.glFrontFace == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFrontFace(a);
#if SYSCALL_LOG
			Util.Log("glFrontFace("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 192:
			if(mIoctls.glGenBuffers == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGenBuffers(a, b);
#if SYSCALL_LOG
			Util.Log("glGenBuffers("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 193:
			if(mIoctls.glGenTextures == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGenTextures(a, b);
#if SYSCALL_LOG
			Util.Log("glGenTextures("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 194:
			if(mIoctls.glGetBooleanv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetBooleanv(a, b);
#if SYSCALL_LOG
			Util.Log("glGetBooleanv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 195:
			if(mIoctls.glGetBufferParameteriv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetBufferParameteriv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetBufferParameteriv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 196:
			if(mIoctls.glGetError == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetError();
#if SYSCALL_LOG
			Util.Log("glGetError("+
				"): "+result+"\n");
#endif
		return result;
			case 197:
			if(mIoctls.glGetFloatv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetFloatv(a, b);
#if SYSCALL_LOG
			Util.Log("glGetFloatv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 198:
			if(mIoctls.glGetIntegerv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetIntegerv(a, b);
#if SYSCALL_LOG
			Util.Log("glGetIntegerv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 199:
			if(mIoctls.glGetStringHandle == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetStringHandle(a);
#if SYSCALL_LOG
			Util.Log("glGetStringHandle("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 200:
			if(mIoctls.glGetTexParameterfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetTexParameterfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetTexParameterfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 201:
			if(mIoctls.glGetTexParameteriv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetTexParameteriv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetTexParameteriv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 202:
			if(mIoctls.glHint == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glHint(a, b);
#if SYSCALL_LOG
			Util.Log("glHint("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 203:
			if(mIoctls.glIsBuffer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glIsBuffer(a);
#if SYSCALL_LOG
			Util.Log("glIsBuffer("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 204:
			if(mIoctls.glIsEnabled == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glIsEnabled(a);
#if SYSCALL_LOG
			Util.Log("glIsEnabled("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 205:
			if(mIoctls.glIsTexture == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glIsTexture(a);
#if SYSCALL_LOG
			Util.Log("glIsTexture("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 206:
			if(mIoctls.glLineWidth == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLineWidth(MoSync.Util.ConvertToFloat(a));
#if SYSCALL_LOG
			Util.Log("glLineWidth("+
				MoSync.Util.ConvertToFloat(a)+
				"): "+result+"\n");
#endif
		return result;
			case 207:
			if(mIoctls.glPixelStorei == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPixelStorei(a, b);
#if SYSCALL_LOG
			Util.Log("glPixelStorei("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 208:
			if(mIoctls.glPolygonOffset == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPolygonOffset(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b));
#if SYSCALL_LOG
			Util.Log("glPolygonOffset("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				"): "+result+"\n");
#endif
		return result;
			case 209:
			if(mIoctls.glReadPixels == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glReadPixels(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8), mCore.GetStackValue(12));
#if SYSCALL_LOG
			Util.Log("glReadPixels("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				","+mCore.GetStackValue(12)+
				"): "+result+"\n");
#endif
		return result;
			case 210:
			if(mIoctls.glSampleCoverage == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glSampleCoverage(MoSync.Util.ConvertToFloat(a), b);
#if SYSCALL_LOG
			Util.Log("glSampleCoverage("+
				MoSync.Util.ConvertToFloat(a)+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 211:
			if(mIoctls.glScissor == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glScissor(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glScissor("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 212:
			if(mIoctls.glStencilFunc == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glStencilFunc(a, b, c);
#if SYSCALL_LOG
			Util.Log("glStencilFunc("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 213:
			if(mIoctls.glStencilMask == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glStencilMask(a);
#if SYSCALL_LOG
			Util.Log("glStencilMask("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 214:
			if(mIoctls.glStencilOp == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glStencilOp(a, b, c);
#if SYSCALL_LOG
			Util.Log("glStencilOp("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 215:
			if(mIoctls.glTexImage2D == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexImage2D(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8), mCore.GetStackValue(12), mCore.GetStackValue(16), mCore.GetStackValue(20));
#if SYSCALL_LOG
			Util.Log("glTexImage2D("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				","+mCore.GetStackValue(12)+
				","+mCore.GetStackValue(16)+
				","+mCore.GetStackValue(20)+
				"): "+result+"\n");
#endif
		return result;
			case 216:
			if(mIoctls.glTexParameterf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexParameterf(a, b, MoSync.Util.ConvertToFloat(c));
#if SYSCALL_LOG
			Util.Log("glTexParameterf("+
				a+
				","+b+
				","+MoSync.Util.ConvertToFloat(c)+
				"): "+result+"\n");
#endif
		return result;
			case 217:
			if(mIoctls.glTexParameterfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexParameterfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexParameterfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 218:
			if(mIoctls.glTexParameteri == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexParameteri(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexParameteri("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 219:
			if(mIoctls.glTexParameteriv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexParameteriv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexParameteriv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 220:
			if(mIoctls.glTexSubImage2D == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexSubImage2D(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8), mCore.GetStackValue(12), mCore.GetStackValue(16), mCore.GetStackValue(20));
#if SYSCALL_LOG
			Util.Log("glTexSubImage2D("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				","+mCore.GetStackValue(12)+
				","+mCore.GetStackValue(16)+
				","+mCore.GetStackValue(20)+
				"): "+result+"\n");
#endif
		return result;
			case 221:
			if(mIoctls.glViewport == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glViewport(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glViewport("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 222:
			if(mIoctls.glAttachShader == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glAttachShader(a, b);
#if SYSCALL_LOG
			Util.Log("glAttachShader("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 223:
			if(mIoctls.glBindAttribLocation == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBindAttribLocation(a, b, c);
#if SYSCALL_LOG
			Util.Log("glBindAttribLocation("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 224:
			if(mIoctls.glBindFramebuffer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBindFramebuffer(a, b);
#if SYSCALL_LOG
			Util.Log("glBindFramebuffer("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 225:
			if(mIoctls.glBindRenderbuffer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBindRenderbuffer(a, b);
#if SYSCALL_LOG
			Util.Log("glBindRenderbuffer("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 226:
			if(mIoctls.glBlendColor == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBlendColor(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)));
#if SYSCALL_LOG
			Util.Log("glBlendColor("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				"): "+result+"\n");
#endif
		return result;
			case 227:
			if(mIoctls.glBlendEquation == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBlendEquation(a);
#if SYSCALL_LOG
			Util.Log("glBlendEquation("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 228:
			if(mIoctls.glBlendEquationSeparate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBlendEquationSeparate(a, b);
#if SYSCALL_LOG
			Util.Log("glBlendEquationSeparate("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 229:
			if(mIoctls.glBlendFuncSeparate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBlendFuncSeparate(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glBlendFuncSeparate("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 230:
			if(mIoctls.glCheckFramebufferStatus == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCheckFramebufferStatus(a);
#if SYSCALL_LOG
			Util.Log("glCheckFramebufferStatus("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 231:
			if(mIoctls.glCompileShader == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCompileShader(a);
#if SYSCALL_LOG
			Util.Log("glCompileShader("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 232:
			if(mIoctls.glCreateProgram == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCreateProgram();
#if SYSCALL_LOG
			Util.Log("glCreateProgram("+
				"): "+result+"\n");
#endif
		return result;
			case 233:
			if(mIoctls.glCreateShader == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCreateShader(a);
#if SYSCALL_LOG
			Util.Log("glCreateShader("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 234:
			if(mIoctls.glDeleteFramebuffers == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDeleteFramebuffers(a, b);
#if SYSCALL_LOG
			Util.Log("glDeleteFramebuffers("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 235:
			if(mIoctls.glDeleteProgram == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDeleteProgram(a);
#if SYSCALL_LOG
			Util.Log("glDeleteProgram("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 236:
			if(mIoctls.glDeleteRenderbuffers == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDeleteRenderbuffers(a, b);
#if SYSCALL_LOG
			Util.Log("glDeleteRenderbuffers("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 237:
			if(mIoctls.glDeleteShader == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDeleteShader(a);
#if SYSCALL_LOG
			Util.Log("glDeleteShader("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 238:
			if(mIoctls.glDetachShader == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDetachShader(a, b);
#if SYSCALL_LOG
			Util.Log("glDetachShader("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 239:
			if(mIoctls.glDisableVertexAttribArray == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDisableVertexAttribArray(a);
#if SYSCALL_LOG
			Util.Log("glDisableVertexAttribArray("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 240:
			if(mIoctls.glEnableVertexAttribArray == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glEnableVertexAttribArray(a);
#if SYSCALL_LOG
			Util.Log("glEnableVertexAttribArray("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 241:
			if(mIoctls.glFramebufferRenderbuffer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFramebufferRenderbuffer(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glFramebufferRenderbuffer("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 242:
			if(mIoctls.glFramebufferTexture2D == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFramebufferTexture2D(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("glFramebufferTexture2D("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 243:
			if(mIoctls.glGenerateMipmap == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGenerateMipmap(a);
#if SYSCALL_LOG
			Util.Log("glGenerateMipmap("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 244:
			if(mIoctls.glGenFramebuffers == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGenFramebuffers(a, b);
#if SYSCALL_LOG
			Util.Log("glGenFramebuffers("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 245:
			if(mIoctls.glGenRenderbuffers == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGenRenderbuffers(a, b);
#if SYSCALL_LOG
			Util.Log("glGenRenderbuffers("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 246:
			if(mIoctls.glGetActiveAttrib == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetActiveAttrib(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8), mCore.GetStackValue(12));
#if SYSCALL_LOG
			Util.Log("glGetActiveAttrib("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				","+mCore.GetStackValue(12)+
				"): "+result+"\n");
#endif
		return result;
			case 247:
			if(mIoctls.glGetActiveUniform == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetActiveUniform(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8), mCore.GetStackValue(12));
#if SYSCALL_LOG
			Util.Log("glGetActiveUniform("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				","+mCore.GetStackValue(12)+
				"): "+result+"\n");
#endif
		return result;
			case 248:
			if(mIoctls.glGetAttachedShaders == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetAttachedShaders(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glGetAttachedShaders("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 249:
			if(mIoctls.glGetAttribLocation == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetAttribLocation(a, b);
#if SYSCALL_LOG
			Util.Log("glGetAttribLocation("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 250:
			if(mIoctls.glGetFramebufferAttachmentParameteriv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetFramebufferAttachmentParameteriv(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glGetFramebufferAttachmentParameteriv("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 251:
			if(mIoctls.glGetProgramiv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetProgramiv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetProgramiv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 252:
			if(mIoctls.glGetProgramInfoLog == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetProgramInfoLog(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glGetProgramInfoLog("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 253:
			if(mIoctls.glGetRenderbufferParameteriv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetRenderbufferParameteriv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetRenderbufferParameteriv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 254:
			if(mIoctls.glGetShaderiv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetShaderiv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetShaderiv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 255:
			if(mIoctls.glGetShaderInfoLog == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetShaderInfoLog(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glGetShaderInfoLog("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 256:
			if(mIoctls.glGetShaderPrecisionFormat == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetShaderPrecisionFormat(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glGetShaderPrecisionFormat("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 257:
			if(mIoctls.glGetShaderSource == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetShaderSource(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glGetShaderSource("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 258:
			if(mIoctls.glGetUniformfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetUniformfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetUniformfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 259:
			if(mIoctls.glGetUniformiv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetUniformiv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetUniformiv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 260:
			if(mIoctls.glGetUniformLocation == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetUniformLocation(a, b);
#if SYSCALL_LOG
			Util.Log("glGetUniformLocation("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 261:
			if(mIoctls.glGetVertexAttribfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetVertexAttribfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetVertexAttribfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 262:
			if(mIoctls.glGetVertexAttribiv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetVertexAttribiv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetVertexAttribiv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 263:
			if(mIoctls.glGetVertexAttribPointerv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetVertexAttribPointerv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetVertexAttribPointerv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 264:
			if(mIoctls.glIsFramebuffer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glIsFramebuffer(a);
#if SYSCALL_LOG
			Util.Log("glIsFramebuffer("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 265:
			if(mIoctls.glIsProgram == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glIsProgram(a);
#if SYSCALL_LOG
			Util.Log("glIsProgram("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 266:
			if(mIoctls.glIsRenderbuffer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glIsRenderbuffer(a);
#if SYSCALL_LOG
			Util.Log("glIsRenderbuffer("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 267:
			if(mIoctls.glIsShader == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glIsShader(a);
#if SYSCALL_LOG
			Util.Log("glIsShader("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 268:
			if(mIoctls.glLinkProgram == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLinkProgram(a);
#if SYSCALL_LOG
			Util.Log("glLinkProgram("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 269:
			if(mIoctls.glReleaseShaderCompiler == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glReleaseShaderCompiler();
#if SYSCALL_LOG
			Util.Log("glReleaseShaderCompiler("+
				"): "+result+"\n");
#endif
		return result;
			case 270:
			if(mIoctls.glRenderbufferStorage == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glRenderbufferStorage(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glRenderbufferStorage("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 271:
			if(mIoctls.glShaderBinary == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glShaderBinary(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("glShaderBinary("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 272:
			if(mIoctls.glShaderSource == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glShaderSource(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glShaderSource("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 273:
			if(mIoctls.glStencilFuncSeparate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glStencilFuncSeparate(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glStencilFuncSeparate("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 274:
			if(mIoctls.glStencilMaskSeparate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glStencilMaskSeparate(a, b);
#if SYSCALL_LOG
			Util.Log("glStencilMaskSeparate("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 275:
			if(mIoctls.glStencilOpSeparate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glStencilOpSeparate(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glStencilOpSeparate("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 276:
			if(mIoctls.glUniform1f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform1f(a, MoSync.Util.ConvertToFloat(b));
#if SYSCALL_LOG
			Util.Log("glUniform1f("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				"): "+result+"\n");
#endif
		return result;
			case 277:
			if(mIoctls.glUniform1fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform1fv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glUniform1fv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 278:
			if(mIoctls.glUniform1i == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform1i(a, b);
#if SYSCALL_LOG
			Util.Log("glUniform1i("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 279:
			if(mIoctls.glUniform1iv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform1iv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glUniform1iv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 280:
			if(mIoctls.glUniform2f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform2f(a, MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c));
#if SYSCALL_LOG
			Util.Log("glUniform2f("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				"): "+result+"\n");
#endif
		return result;
			case 281:
			if(mIoctls.glUniform2fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform2fv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glUniform2fv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 282:
			if(mIoctls.glUniform2i == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform2i(a, b, c);
#if SYSCALL_LOG
			Util.Log("glUniform2i("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 283:
			if(mIoctls.glUniform2iv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform2iv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glUniform2iv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 284:
			if(mIoctls.glUniform3f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform3f(a, MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)));
#if SYSCALL_LOG
			Util.Log("glUniform3f("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				"): "+result+"\n");
#endif
		return result;
			case 285:
			if(mIoctls.glUniform3fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform3fv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glUniform3fv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 286:
			if(mIoctls.glUniform3i == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform3i(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glUniform3i("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 287:
			if(mIoctls.glUniform3iv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform3iv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glUniform3iv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 288:
			if(mIoctls.glUniform4f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform4f(a, MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)), MoSync.Util.ConvertToFloat(mCore.GetStackValue(4)));
#if SYSCALL_LOG
			Util.Log("glUniform4f("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(4))+
				"): "+result+"\n");
#endif
		return result;
			case 289:
			if(mIoctls.glUniform4fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform4fv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glUniform4fv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 290:
			if(mIoctls.glUniform4i == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform4i(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("glUniform4i("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 291:
			if(mIoctls.glUniform4iv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniform4iv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glUniform4iv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 292:
			if(mIoctls.glUniformMatrix2fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniformMatrix2fv(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glUniformMatrix2fv("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 293:
			if(mIoctls.glUniformMatrix3fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniformMatrix3fv(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glUniformMatrix3fv("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 294:
			if(mIoctls.glUniformMatrix4fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUniformMatrix4fv(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glUniformMatrix4fv("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 295:
			if(mIoctls.glUseProgram == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glUseProgram(a);
#if SYSCALL_LOG
			Util.Log("glUseProgram("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 296:
			if(mIoctls.glValidateProgram == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glValidateProgram(a);
#if SYSCALL_LOG
			Util.Log("glValidateProgram("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 297:
			if(mIoctls.glVertexAttrib1f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexAttrib1f(a, MoSync.Util.ConvertToFloat(b));
#if SYSCALL_LOG
			Util.Log("glVertexAttrib1f("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				"): "+result+"\n");
#endif
		return result;
			case 298:
			if(mIoctls.glVertexAttrib1fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexAttrib1fv(a, b);
#if SYSCALL_LOG
			Util.Log("glVertexAttrib1fv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 299:
			if(mIoctls.glVertexAttrib2f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexAttrib2f(a, MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c));
#if SYSCALL_LOG
			Util.Log("glVertexAttrib2f("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				"): "+result+"\n");
#endif
		return result;
			case 300:
			if(mIoctls.glVertexAttrib2fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexAttrib2fv(a, b);
#if SYSCALL_LOG
			Util.Log("glVertexAttrib2fv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 301:
			if(mIoctls.glVertexAttrib3f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexAttrib3f(a, MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)));
#if SYSCALL_LOG
			Util.Log("glVertexAttrib3f("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				"): "+result+"\n");
#endif
		return result;
			case 302:
			if(mIoctls.glVertexAttrib3fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexAttrib3fv(a, b);
#if SYSCALL_LOG
			Util.Log("glVertexAttrib3fv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 303:
			if(mIoctls.glVertexAttrib4f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexAttrib4f(a, MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)), MoSync.Util.ConvertToFloat(mCore.GetStackValue(4)));
#if SYSCALL_LOG
			Util.Log("glVertexAttrib4f("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(4))+
				"): "+result+"\n");
#endif
		return result;
			case 304:
			if(mIoctls.glVertexAttrib4fv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexAttrib4fv(a, b);
#if SYSCALL_LOG
			Util.Log("glVertexAttrib4fv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 305:
			if(mIoctls.glVertexAttribPointer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexAttribPointer(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8));
#if SYSCALL_LOG
			Util.Log("glVertexAttribPointer("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				"): "+result+"\n");
#endif
		return result;
			case 306:
			if(mIoctls.glAlphaFunc == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glAlphaFunc(a, MoSync.Util.ConvertToFloat(b));
#if SYSCALL_LOG
			Util.Log("glAlphaFunc("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				"): "+result+"\n");
#endif
		return result;
			case 307:
			if(mIoctls.glClipPlanef == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glClipPlanef(a, b);
#if SYSCALL_LOG
			Util.Log("glClipPlanef("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 308:
			if(mIoctls.glColor4f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glColor4f(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)));
#if SYSCALL_LOG
			Util.Log("glColor4f("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				"): "+result+"\n");
#endif
		return result;
			case 309:
			if(mIoctls.glFogf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFogf(a, MoSync.Util.ConvertToFloat(b));
#if SYSCALL_LOG
			Util.Log("glFogf("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				"): "+result+"\n");
#endif
		return result;
			case 310:
			if(mIoctls.glFogfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFogfv(a, b);
#if SYSCALL_LOG
			Util.Log("glFogfv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 311:
			if(mIoctls.glFrustumf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFrustumf(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)), MoSync.Util.ConvertToFloat(mCore.GetStackValue(4)), MoSync.Util.ConvertToFloat(mCore.GetStackValue(8)));
#if SYSCALL_LOG
			Util.Log("glFrustumf("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(4))+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(8))+
				"): "+result+"\n");
#endif
		return result;
			case 312:
			if(mIoctls.glGetClipPlanef == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetClipPlanef(a, b);
#if SYSCALL_LOG
			Util.Log("glGetClipPlanef("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 313:
			if(mIoctls.glGetLightfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetLightfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetLightfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 314:
			if(mIoctls.glGetMaterialfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetMaterialfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetMaterialfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 315:
			if(mIoctls.glGetTexEnvfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetTexEnvfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetTexEnvfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 316:
			if(mIoctls.glLightModelf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLightModelf(a, MoSync.Util.ConvertToFloat(b));
#if SYSCALL_LOG
			Util.Log("glLightModelf("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				"): "+result+"\n");
#endif
		return result;
			case 317:
			if(mIoctls.glLightModelfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLightModelfv(a, b);
#if SYSCALL_LOG
			Util.Log("glLightModelfv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 318:
			if(mIoctls.glLightf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLightf(a, b, MoSync.Util.ConvertToFloat(c));
#if SYSCALL_LOG
			Util.Log("glLightf("+
				a+
				","+b+
				","+MoSync.Util.ConvertToFloat(c)+
				"): "+result+"\n");
#endif
		return result;
			case 319:
			if(mIoctls.glLightfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLightfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glLightfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 320:
			if(mIoctls.glLoadMatrixf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLoadMatrixf(a);
#if SYSCALL_LOG
			Util.Log("glLoadMatrixf("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 321:
			if(mIoctls.glMaterialf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glMaterialf(a, b, MoSync.Util.ConvertToFloat(c));
#if SYSCALL_LOG
			Util.Log("glMaterialf("+
				a+
				","+b+
				","+MoSync.Util.ConvertToFloat(c)+
				"): "+result+"\n");
#endif
		return result;
			case 322:
			if(mIoctls.glMaterialfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glMaterialfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glMaterialfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 323:
			if(mIoctls.glMultMatrixf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glMultMatrixf(a);
#if SYSCALL_LOG
			Util.Log("glMultMatrixf("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 324:
			if(mIoctls.glMultiTexCoord4f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glMultiTexCoord4f(a, MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)), MoSync.Util.ConvertToFloat(mCore.GetStackValue(4)));
#if SYSCALL_LOG
			Util.Log("glMultiTexCoord4f("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(4))+
				"): "+result+"\n");
#endif
		return result;
			case 325:
			if(mIoctls.glNormal3f == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glNormal3f(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c));
#if SYSCALL_LOG
			Util.Log("glNormal3f("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				"): "+result+"\n");
#endif
		return result;
			case 326:
			if(mIoctls.glOrthof == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glOrthof(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)), MoSync.Util.ConvertToFloat(mCore.GetStackValue(4)), MoSync.Util.ConvertToFloat(mCore.GetStackValue(8)));
#if SYSCALL_LOG
			Util.Log("glOrthof("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(4))+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(8))+
				"): "+result+"\n");
#endif
		return result;
			case 327:
			if(mIoctls.glPointParameterf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPointParameterf(a, MoSync.Util.ConvertToFloat(b));
#if SYSCALL_LOG
			Util.Log("glPointParameterf("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				"): "+result+"\n");
#endif
		return result;
			case 328:
			if(mIoctls.glPointParameterfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPointParameterfv(a, b);
#if SYSCALL_LOG
			Util.Log("glPointParameterfv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 329:
			if(mIoctls.glPointSize == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPointSize(MoSync.Util.ConvertToFloat(a));
#if SYSCALL_LOG
			Util.Log("glPointSize("+
				MoSync.Util.ConvertToFloat(a)+
				"): "+result+"\n");
#endif
		return result;
			case 330:
			if(mIoctls.glRotatef == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glRotatef(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c), MoSync.Util.ConvertToFloat(mCore.GetStackValue(0)));
#if SYSCALL_LOG
			Util.Log("glRotatef("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				","+MoSync.Util.ConvertToFloat(mCore.GetStackValue(0))+
				"): "+result+"\n");
#endif
		return result;
			case 331:
			if(mIoctls.glScalef == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glScalef(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c));
#if SYSCALL_LOG
			Util.Log("glScalef("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				"): "+result+"\n");
#endif
		return result;
			case 332:
			if(mIoctls.glTexEnvf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexEnvf(a, b, MoSync.Util.ConvertToFloat(c));
#if SYSCALL_LOG
			Util.Log("glTexEnvf("+
				a+
				","+b+
				","+MoSync.Util.ConvertToFloat(c)+
				"): "+result+"\n");
#endif
		return result;
			case 333:
			if(mIoctls.glTexEnvfv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexEnvfv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexEnvfv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 334:
			if(mIoctls.glTranslatef == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTranslatef(MoSync.Util.ConvertToFloat(a), MoSync.Util.ConvertToFloat(b), MoSync.Util.ConvertToFloat(c));
#if SYSCALL_LOG
			Util.Log("glTranslatef("+
				MoSync.Util.ConvertToFloat(a)+
				","+MoSync.Util.ConvertToFloat(b)+
				","+MoSync.Util.ConvertToFloat(c)+
				"): "+result+"\n");
#endif
		return result;
			case 335:
			if(mIoctls.glAlphaFuncx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glAlphaFuncx(a, b);
#if SYSCALL_LOG
			Util.Log("glAlphaFuncx("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 336:
			if(mIoctls.glClearColorx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glClearColorx(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glClearColorx("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 337:
			if(mIoctls.glClearDepthx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glClearDepthx(a);
#if SYSCALL_LOG
			Util.Log("glClearDepthx("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 338:
			if(mIoctls.glClientActiveTexture == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glClientActiveTexture(a);
#if SYSCALL_LOG
			Util.Log("glClientActiveTexture("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 339:
			if(mIoctls.glClipPlanex == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glClipPlanex(a, b);
#if SYSCALL_LOG
			Util.Log("glClipPlanex("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 340:
			if(mIoctls.glColor4ub == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glColor4ub(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glColor4ub("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 341:
			if(mIoctls.glColor4x == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glColor4x(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glColor4x("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 342:
			if(mIoctls.glColorPointer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glColorPointer(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glColorPointer("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 343:
			if(mIoctls.glDepthRangex == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDepthRangex(a, b);
#if SYSCALL_LOG
			Util.Log("glDepthRangex("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 344:
			if(mIoctls.glDisableClientState == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDisableClientState(a);
#if SYSCALL_LOG
			Util.Log("glDisableClientState("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 345:
			if(mIoctls.glEnableClientState == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glEnableClientState(a);
#if SYSCALL_LOG
			Util.Log("glEnableClientState("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 346:
			if(mIoctls.glFogx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFogx(a, b);
#if SYSCALL_LOG
			Util.Log("glFogx("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 347:
			if(mIoctls.glFogxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFogxv(a, b);
#if SYSCALL_LOG
			Util.Log("glFogxv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 348:
			if(mIoctls.glFrustumx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFrustumx(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8));
#if SYSCALL_LOG
			Util.Log("glFrustumx("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				"): "+result+"\n");
#endif
		return result;
			case 349:
			if(mIoctls.glGetClipPlanex == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetClipPlanex(a, b);
#if SYSCALL_LOG
			Util.Log("glGetClipPlanex("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 350:
			if(mIoctls.glGetFixedv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetFixedv(a, b);
#if SYSCALL_LOG
			Util.Log("glGetFixedv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 351:
			if(mIoctls.glGetLightxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetLightxv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetLightxv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 352:
			if(mIoctls.glGetMaterialxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetMaterialxv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetMaterialxv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 353:
			if(mIoctls.glGetPointerv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetPointerv(a, b);
#if SYSCALL_LOG
			Util.Log("glGetPointerv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 354:
			if(mIoctls.glGetTexEnviv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetTexEnviv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetTexEnviv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 355:
			if(mIoctls.glGetTexEnvxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetTexEnvxv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetTexEnvxv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 356:
			if(mIoctls.glGetTexParameterxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetTexParameterxv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetTexParameterxv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 357:
			if(mIoctls.glLightModelx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLightModelx(a, b);
#if SYSCALL_LOG
			Util.Log("glLightModelx("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 358:
			if(mIoctls.glLightModelxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLightModelxv(a, b);
#if SYSCALL_LOG
			Util.Log("glLightModelxv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 359:
			if(mIoctls.glLightx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLightx(a, b, c);
#if SYSCALL_LOG
			Util.Log("glLightx("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 360:
			if(mIoctls.glLightxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLightxv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glLightxv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 361:
			if(mIoctls.glLineWidthx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLineWidthx(a);
#if SYSCALL_LOG
			Util.Log("glLineWidthx("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 362:
			if(mIoctls.glLoadIdentity == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLoadIdentity();
#if SYSCALL_LOG
			Util.Log("glLoadIdentity("+
				"): "+result+"\n");
#endif
		return result;
			case 363:
			if(mIoctls.glLoadMatrixx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLoadMatrixx(a);
#if SYSCALL_LOG
			Util.Log("glLoadMatrixx("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 364:
			if(mIoctls.glLogicOp == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glLogicOp(a);
#if SYSCALL_LOG
			Util.Log("glLogicOp("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 365:
			if(mIoctls.glMaterialx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glMaterialx(a, b, c);
#if SYSCALL_LOG
			Util.Log("glMaterialx("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 366:
			if(mIoctls.glMaterialxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glMaterialxv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glMaterialxv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 367:
			if(mIoctls.glMatrixMode == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glMatrixMode(a);
#if SYSCALL_LOG
			Util.Log("glMatrixMode("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 368:
			if(mIoctls.glMultMatrixx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glMultMatrixx(a);
#if SYSCALL_LOG
			Util.Log("glMultMatrixx("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 369:
			if(mIoctls.glMultiTexCoord4x == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glMultiTexCoord4x(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("glMultiTexCoord4x("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 370:
			if(mIoctls.glNormal3x == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glNormal3x(a, b, c);
#if SYSCALL_LOG
			Util.Log("glNormal3x("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 371:
			if(mIoctls.glNormalPointer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glNormalPointer(a, b, c);
#if SYSCALL_LOG
			Util.Log("glNormalPointer("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 372:
			if(mIoctls.glOrthox == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glOrthox(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4), mCore.GetStackValue(8));
#if SYSCALL_LOG
			Util.Log("glOrthox("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				","+mCore.GetStackValue(8)+
				"): "+result+"\n");
#endif
		return result;
			case 373:
			if(mIoctls.glPointParameterx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPointParameterx(a, b);
#if SYSCALL_LOG
			Util.Log("glPointParameterx("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 374:
			if(mIoctls.glPointParameterxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPointParameterxv(a, b);
#if SYSCALL_LOG
			Util.Log("glPointParameterxv("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 375:
			if(mIoctls.glPointSizex == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPointSizex(a);
#if SYSCALL_LOG
			Util.Log("glPointSizex("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 376:
			if(mIoctls.glPolygonOffsetx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPolygonOffsetx(a, b);
#if SYSCALL_LOG
			Util.Log("glPolygonOffsetx("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 377:
			if(mIoctls.glPopMatrix == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPopMatrix();
#if SYSCALL_LOG
			Util.Log("glPopMatrix("+
				"): "+result+"\n");
#endif
		return result;
			case 378:
			if(mIoctls.glPushMatrix == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPushMatrix();
#if SYSCALL_LOG
			Util.Log("glPushMatrix("+
				"): "+result+"\n");
#endif
		return result;
			case 379:
			if(mIoctls.glRotatex == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glRotatex(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glRotatex("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 380:
			if(mIoctls.glSampleCoveragex == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glSampleCoveragex(a, b);
#if SYSCALL_LOG
			Util.Log("glSampleCoveragex("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 381:
			if(mIoctls.glScalex == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glScalex(a, b, c);
#if SYSCALL_LOG
			Util.Log("glScalex("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 382:
			if(mIoctls.glShadeModel == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glShadeModel(a);
#if SYSCALL_LOG
			Util.Log("glShadeModel("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 383:
			if(mIoctls.glTexCoordPointer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexCoordPointer(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glTexCoordPointer("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 384:
			if(mIoctls.glTexEnvi == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexEnvi(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexEnvi("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 385:
			if(mIoctls.glTexEnvx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexEnvx(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexEnvx("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 386:
			if(mIoctls.glTexEnviv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexEnviv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexEnviv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 387:
			if(mIoctls.glTexEnvxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexEnvxv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexEnvxv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 388:
			if(mIoctls.glTexParameterx == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexParameterx(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexParameterx("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 389:
			if(mIoctls.glTexParameterxv == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTexParameterxv(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTexParameterxv("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 390:
			if(mIoctls.glTranslatex == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glTranslatex(a, b, c);
#if SYSCALL_LOG
			Util.Log("glTranslatex("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 391:
			if(mIoctls.glVertexPointer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glVertexPointer(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glVertexPointer("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 392:
			if(mIoctls.glPointSizePointerOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glPointSizePointerOES(a, b, c);
#if SYSCALL_LOG
			Util.Log("glPointSizePointerOES("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 393:
			if(mIoctls.glIsRenderbufferOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glIsRenderbufferOES(a);
#if SYSCALL_LOG
			Util.Log("glIsRenderbufferOES("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 394:
			if(mIoctls.glBindRenderbufferOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBindRenderbufferOES(a, b);
#if SYSCALL_LOG
			Util.Log("glBindRenderbufferOES("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 395:
			if(mIoctls.glDeleteRenderbuffersOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDeleteRenderbuffersOES(a, b);
#if SYSCALL_LOG
			Util.Log("glDeleteRenderbuffersOES("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 396:
			if(mIoctls.glGenRenderbuffersOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGenRenderbuffersOES(a, b);
#if SYSCALL_LOG
			Util.Log("glGenRenderbuffersOES("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 397:
			if(mIoctls.glRenderbufferStorageOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glRenderbufferStorageOES(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glRenderbufferStorageOES("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 398:
			if(mIoctls.glGetRenderbufferParameterivOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetRenderbufferParameterivOES(a, b, c);
#if SYSCALL_LOG
			Util.Log("glGetRenderbufferParameterivOES("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 399:
			if(mIoctls.glIsFramebufferOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glIsFramebufferOES(a);
#if SYSCALL_LOG
			Util.Log("glIsFramebufferOES("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 400:
			if(mIoctls.glBindFramebufferOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glBindFramebufferOES(a, b);
#if SYSCALL_LOG
			Util.Log("glBindFramebufferOES("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 401:
			if(mIoctls.glDeleteFramebuffersOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glDeleteFramebuffersOES(a, b);
#if SYSCALL_LOG
			Util.Log("glDeleteFramebuffersOES("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 402:
			if(mIoctls.glGenFramebuffersOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGenFramebuffersOES(a, b);
#if SYSCALL_LOG
			Util.Log("glGenFramebuffersOES("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 403:
			if(mIoctls.glCheckFramebufferStatusOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glCheckFramebufferStatusOES(a);
#if SYSCALL_LOG
			Util.Log("glCheckFramebufferStatusOES("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 404:
			if(mIoctls.glFramebufferRenderbufferOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFramebufferRenderbufferOES(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glFramebufferRenderbufferOES("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 405:
			if(mIoctls.glFramebufferTexture2DOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glFramebufferTexture2DOES(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("glFramebufferTexture2DOES("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 406:
			if(mIoctls.glGetFramebufferAttachmentParameterivOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGetFramebufferAttachmentParameterivOES(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("glGetFramebufferAttachmentParameterivOES("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 407:
			if(mIoctls.glGenerateMipmapOES == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.glGenerateMipmapOES(a);
#if SYSCALL_LOG
			Util.Log("glGenerateMipmapOES("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 408:
			if(mIoctls.maReportResourceInformation == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maReportResourceInformation();
#if SYSCALL_LOG
			Util.Log("maReportResourceInformation("+
				"): "+result+"\n");
#endif
		return result;
			case 409:
			if(mIoctls.maMessageBox == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maMessageBox(a, b);
#if SYSCALL_LOG
			Util.Log("maMessageBox("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 410:
			if(mIoctls.maAlert == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAlert(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("maAlert("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 411:
			if(mIoctls.maImagePickerOpen == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maImagePickerOpen();
#if SYSCALL_LOG
			Util.Log("maImagePickerOpen("+
				"): "+result+"\n");
#endif
		return result;
			case 412:
			if(mIoctls.maOptionsBox == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maOptionsBox(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("maOptionsBox("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 413:
			if(mIoctls.maSensorStart == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSensorStart(a, b);
#if SYSCALL_LOG
			Util.Log("maSensorStart("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 414:
			if(mIoctls.maSensorStop == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSensorStop(a);
#if SYSCALL_LOG
			Util.Log("maSensorStop("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 415:
			if(mIoctls.maNFCStart == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCStart();
#if SYSCALL_LOG
			Util.Log("maNFCStart("+
				"): "+result+"\n");
#endif
		return result;
			case 416:
			if(mIoctls.maNFCStop == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCStop();
#if SYSCALL_LOG
			Util.Log("maNFCStop("+
				"): "+result+"\n");
#endif
		return result;
			case 417:
			if(mIoctls.maNFCReadTag == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCReadTag(a);
#if SYSCALL_LOG
			Util.Log("maNFCReadTag("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 418:
			if(mIoctls.maNFCDestroyTag == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCDestroyTag(a);
#if SYSCALL_LOG
			Util.Log("maNFCDestroyTag("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 419:
			if(mIoctls.maNFCConnectTag == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCConnectTag(a);
#if SYSCALL_LOG
			Util.Log("maNFCConnectTag("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 420:
			if(mIoctls.maNFCCloseTag == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCCloseTag(a);
#if SYSCALL_LOG
			Util.Log("maNFCCloseTag("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 421:
			if(mIoctls.maNFCIsType == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCIsType(a, b);
#if SYSCALL_LOG
			Util.Log("maNFCIsType("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 422:
			if(mIoctls.maNFCGetTypedTag == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetTypedTag(a, b);
#if SYSCALL_LOG
			Util.Log("maNFCGetTypedTag("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 423:
			if(mIoctls.maNFCBatchStart == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCBatchStart(a);
#if SYSCALL_LOG
			Util.Log("maNFCBatchStart("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 424:
			if(mIoctls.maNFCBatchCommit == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCBatchCommit(a);
#if SYSCALL_LOG
			Util.Log("maNFCBatchCommit("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 425:
			if(mIoctls.maNFCBatchRollback == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCBatchRollback(a);
#if SYSCALL_LOG
			Util.Log("maNFCBatchRollback("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 426:
			if(mIoctls.maNFCTransceive == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCTransceive(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("maNFCTransceive("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 427:
			if(mIoctls.maNFCSetReadOnly == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCSetReadOnly(a);
#if SYSCALL_LOG
			Util.Log("maNFCSetReadOnly("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 428:
			if(mIoctls.maNFCIsReadOnly == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCIsReadOnly(a);
#if SYSCALL_LOG
			Util.Log("maNFCIsReadOnly("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 429:
			if(mIoctls.maNFCGetSize == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetSize(a);
#if SYSCALL_LOG
			Util.Log("maNFCGetSize("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 430:
			if(mIoctls.maNFCGetId == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetId(a, b, c);
#if SYSCALL_LOG
			Util.Log("maNFCGetId("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 431:
			if(mIoctls.maNFCReadNDEFMessage == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCReadNDEFMessage(a);
#if SYSCALL_LOG
			Util.Log("maNFCReadNDEFMessage("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 432:
			if(mIoctls.maNFCWriteNDEFMessage == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCWriteNDEFMessage(a, b);
#if SYSCALL_LOG
			Util.Log("maNFCWriteNDEFMessage("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 433:
			if(mIoctls.maNFCCreateNDEFMessage == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCCreateNDEFMessage(a);
#if SYSCALL_LOG
			Util.Log("maNFCCreateNDEFMessage("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 434:
			if(mIoctls.maNFCGetNDEFMessage == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetNDEFMessage(a);
#if SYSCALL_LOG
			Util.Log("maNFCGetNDEFMessage("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 435:
			if(mIoctls.maNFCGetNDEFRecord == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetNDEFRecord(a, b);
#if SYSCALL_LOG
			Util.Log("maNFCGetNDEFRecord("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 436:
			if(mIoctls.maNFCGetNDEFRecordCount == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetNDEFRecordCount(a);
#if SYSCALL_LOG
			Util.Log("maNFCGetNDEFRecordCount("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 437:
			if(mIoctls.maNFCGetNDEFId == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetNDEFId(a, b, c);
#if SYSCALL_LOG
			Util.Log("maNFCGetNDEFId("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 438:
			if(mIoctls.maNFCGetNDEFPayload == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetNDEFPayload(a, b, c);
#if SYSCALL_LOG
			Util.Log("maNFCGetNDEFPayload("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 439:
			if(mIoctls.maNFCGetNDEFTnf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetNDEFTnf(a);
#if SYSCALL_LOG
			Util.Log("maNFCGetNDEFTnf("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 440:
			if(mIoctls.maNFCGetNDEFType == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetNDEFType(a, b, c);
#if SYSCALL_LOG
			Util.Log("maNFCGetNDEFType("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 441:
			if(mIoctls.maNFCSetNDEFId == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCSetNDEFId(a, b, c);
#if SYSCALL_LOG
			Util.Log("maNFCSetNDEFId("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 442:
			if(mIoctls.maNFCSetNDEFPayload == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCSetNDEFPayload(a, b, c);
#if SYSCALL_LOG
			Util.Log("maNFCSetNDEFPayload("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 443:
			if(mIoctls.maNFCSetNDEFTnf == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCSetNDEFTnf(a, b);
#if SYSCALL_LOG
			Util.Log("maNFCSetNDEFTnf("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 444:
			if(mIoctls.maNFCSetNDEFType == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCSetNDEFType(a, b, c);
#if SYSCALL_LOG
			Util.Log("maNFCSetNDEFType("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 445:
			if(mIoctls.maNFCAuthenticateMifareSector == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCAuthenticateMifareSector(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("maNFCAuthenticateMifareSector("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 446:
			if(mIoctls.maNFCGetMifareSectorCount == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetMifareSectorCount(a);
#if SYSCALL_LOG
			Util.Log("maNFCGetMifareSectorCount("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 447:
			if(mIoctls.maNFCGetMifareBlockCountInSector == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCGetMifareBlockCountInSector(a, b);
#if SYSCALL_LOG
			Util.Log("maNFCGetMifareBlockCountInSector("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 448:
			if(mIoctls.maNFCMifareSectorToBlock == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCMifareSectorToBlock(a, b);
#if SYSCALL_LOG
			Util.Log("maNFCMifareSectorToBlock("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 449:
			if(mIoctls.maNFCReadMifareBlocks == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCReadMifareBlocks(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maNFCReadMifareBlocks("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 450:
			if(mIoctls.maNFCWriteMifareBlocks == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCWriteMifareBlocks(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maNFCWriteMifareBlocks("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 451:
			if(mIoctls.maNFCReadMifarePages == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCReadMifarePages(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maNFCReadMifarePages("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 452:
			if(mIoctls.maNFCWriteMifarePages == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNFCWriteMifarePages(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maNFCWriteMifarePages("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 453:
			if(mIoctls.maDBOpen == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBOpen(a);
#if SYSCALL_LOG
			Util.Log("maDBOpen("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 454:
			if(mIoctls.maDBClose == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBClose(a);
#if SYSCALL_LOG
			Util.Log("maDBClose("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 455:
			if(mIoctls.maDBExecSQL == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBExecSQL(a, b);
#if SYSCALL_LOG
			Util.Log("maDBExecSQL("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 456:
			if(mIoctls.maDBExecSQLParams == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBExecSQLParams(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maDBExecSQLParams("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 457:
			if(mIoctls.maDBCursorDestroy == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBCursorDestroy(a);
#if SYSCALL_LOG
			Util.Log("maDBCursorDestroy("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 458:
			if(mIoctls.maDBCursorNext == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBCursorNext(a);
#if SYSCALL_LOG
			Util.Log("maDBCursorNext("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 459:
			if(mIoctls.maDBCursorGetColumnData == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBCursorGetColumnData(a, b, c);
#if SYSCALL_LOG
			Util.Log("maDBCursorGetColumnData("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 460:
			if(mIoctls.maDBCursorGetColumnText == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBCursorGetColumnText(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maDBCursorGetColumnText("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 461:
			if(mIoctls.maDBCursorGetColumnInt == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBCursorGetColumnInt(a, b, c);
#if SYSCALL_LOG
			Util.Log("maDBCursorGetColumnInt("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 462:
			if(mIoctls.maDBCursorGetColumnDouble == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maDBCursorGetColumnDouble(a, b, c);
#if SYSCALL_LOG
			Util.Log("maDBCursorGetColumnDouble("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 463:
			if(mIoctls.maAudioDataCreateFromResource == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioDataCreateFromResource(a, b, c, mCore.GetStackValue(0), mCore.GetStackValue(4));
#if SYSCALL_LOG
			Util.Log("maAudioDataCreateFromResource("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				","+mCore.GetStackValue(4)+
				"): "+result+"\n");
#endif
		return result;
			case 464:
			if(mIoctls.maAudioDataCreateFromURL == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioDataCreateFromURL(a, b, c);
#if SYSCALL_LOG
			Util.Log("maAudioDataCreateFromURL("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 465:
			if(mIoctls.maAudioDataDestroy == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioDataDestroy(a);
#if SYSCALL_LOG
			Util.Log("maAudioDataDestroy("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 466:
			if(mIoctls.maAudioInstanceCreate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioInstanceCreate(a);
#if SYSCALL_LOG
			Util.Log("maAudioInstanceCreate("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 467:
			if(mIoctls.maAudioInstanceCreateDynamic == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioInstanceCreateDynamic(a, b, c);
#if SYSCALL_LOG
			Util.Log("maAudioInstanceCreateDynamic("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 468:
			if(mIoctls.maAudioSubmitBuffer == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioSubmitBuffer(a, b, c);
#if SYSCALL_LOG
			Util.Log("maAudioSubmitBuffer("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 469:
			if(mIoctls.maAudioGetPendingBufferCount == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioGetPendingBufferCount(a);
#if SYSCALL_LOG
			Util.Log("maAudioGetPendingBufferCount("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 470:
			if(mIoctls.maAudioInstanceDestroy == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioInstanceDestroy(a);
#if SYSCALL_LOG
			Util.Log("maAudioInstanceDestroy("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 471:
			if(mIoctls.maAudioGetLength == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioGetLength(a);
#if SYSCALL_LOG
			Util.Log("maAudioGetLength("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 472:
			if(mIoctls.maAudioSetNumberOfLoops == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioSetNumberOfLoops(a, b);
#if SYSCALL_LOG
			Util.Log("maAudioSetNumberOfLoops("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 473:
			if(mIoctls.maAudioPrepare == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioPrepare(a, b);
#if SYSCALL_LOG
			Util.Log("maAudioPrepare("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 474:
			if(mIoctls.maAudioPlay == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioPlay(a);
#if SYSCALL_LOG
			Util.Log("maAudioPlay("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 475:
			if(mIoctls.maAudioSetPosition == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioSetPosition(a, b);
#if SYSCALL_LOG
			Util.Log("maAudioSetPosition("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 476:
			if(mIoctls.maAudioGetPosition == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioGetPosition(a);
#if SYSCALL_LOG
			Util.Log("maAudioGetPosition("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 477:
			if(mIoctls.maAudioSetVolume == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioSetVolume(a, MoSync.Util.ConvertToFloat(b));
#if SYSCALL_LOG
			Util.Log("maAudioSetVolume("+
				a+
				","+MoSync.Util.ConvertToFloat(b)+
				"): "+result+"\n");
#endif
		return result;
			case 478:
			if(mIoctls.maAudioPause == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioPause(a);
#if SYSCALL_LOG
			Util.Log("maAudioPause("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 479:
			if(mIoctls.maAudioStop == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAudioStop(a);
#if SYSCALL_LOG
			Util.Log("maAudioStop("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 480:
			if(mIoctls.maSyscallPanicsEnable == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSyscallPanicsEnable();
#if SYSCALL_LOG
			Util.Log("maSyscallPanicsEnable("+
				"): "+result+"\n");
#endif
		return result;
			case 481:
			if(mIoctls.maSyscallPanicsDisable == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maSyscallPanicsDisable();
#if SYSCALL_LOG
			Util.Log("maSyscallPanicsDisable("+
				"): "+result+"\n");
#endif
		return result;
			case 482:
			if(mIoctls.maAdsBannerCreate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAdsBannerCreate(a, b);
#if SYSCALL_LOG
			Util.Log("maAdsBannerCreate("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 483:
			if(mIoctls.maAdsBannerDestroy == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAdsBannerDestroy(a);
#if SYSCALL_LOG
			Util.Log("maAdsBannerDestroy("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 484:
			if(mIoctls.maAdsAddBannerToLayout == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAdsAddBannerToLayout(a, b);
#if SYSCALL_LOG
			Util.Log("maAdsAddBannerToLayout("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 485:
			if(mIoctls.maAdsRemoveBannerFromLayout == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAdsRemoveBannerFromLayout(a, b);
#if SYSCALL_LOG
			Util.Log("maAdsRemoveBannerFromLayout("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 486:
			if(mIoctls.maAdsBannerSetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAdsBannerSetProperty(a, b, c);
#if SYSCALL_LOG
			Util.Log("maAdsBannerSetProperty("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 487:
			if(mIoctls.maAdsBannerGetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maAdsBannerGetProperty(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maAdsBannerGetProperty("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 488:
			if(mIoctls.maNotificationLocalCreate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationLocalCreate();
#if SYSCALL_LOG
			Util.Log("maNotificationLocalCreate("+
				"): "+result+"\n");
#endif
		return result;
			case 489:
			if(mIoctls.maNotificationLocalDestroy == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationLocalDestroy(a);
#if SYSCALL_LOG
			Util.Log("maNotificationLocalDestroy("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 490:
			if(mIoctls.maNotificationLocalSetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationLocalSetProperty(a, b, c);
#if SYSCALL_LOG
			Util.Log("maNotificationLocalSetProperty("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 491:
			if(mIoctls.maNotificationLocalGetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationLocalGetProperty(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maNotificationLocalGetProperty("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 492:
			if(mIoctls.maNotificationLocalSchedule == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationLocalSchedule(a);
#if SYSCALL_LOG
			Util.Log("maNotificationLocalSchedule("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 493:
			if(mIoctls.maNotificationLocalUnschedule == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationLocalUnschedule(a);
#if SYSCALL_LOG
			Util.Log("maNotificationLocalUnschedule("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 494:
			if(mIoctls.maNotificationPushRegister == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationPushRegister(a, b);
#if SYSCALL_LOG
			Util.Log("maNotificationPushRegister("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 495:
			if(mIoctls.maNotificationPushUnregister == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationPushUnregister();
#if SYSCALL_LOG
			Util.Log("maNotificationPushUnregister("+
				"): "+result+"\n");
#endif
		return result;
			case 496:
			if(mIoctls.maNotificationPushGetRegistration == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationPushGetRegistration(a, b);
#if SYSCALL_LOG
			Util.Log("maNotificationPushGetRegistration("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 497:
			if(mIoctls.maNotificationPushGetData == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationPushGetData(a, b);
#if SYSCALL_LOG
			Util.Log("maNotificationPushGetData("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 498:
			if(mIoctls.maNotificationPushDestroy == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationPushDestroy(a);
#if SYSCALL_LOG
			Util.Log("maNotificationPushDestroy("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 499:
			if(mIoctls.maNotificationSetIconBadge == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationSetIconBadge(a);
#if SYSCALL_LOG
			Util.Log("maNotificationSetIconBadge("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 500:
			if(mIoctls.maNotificationGetIconBadge == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationGetIconBadge();
#if SYSCALL_LOG
			Util.Log("maNotificationGetIconBadge("+
				"): "+result+"\n");
#endif
		return result;
			case 501:
			if(mIoctls.maNotificationPushSetTickerText == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationPushSetTickerText(a);
#if SYSCALL_LOG
			Util.Log("maNotificationPushSetTickerText("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 502:
			if(mIoctls.maNotificationPushSetMessageTitle == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationPushSetMessageTitle(a);
#if SYSCALL_LOG
			Util.Log("maNotificationPushSetMessageTitle("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 503:
			if(mIoctls.maNotificationPushSetDisplayFlag == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maNotificationPushSetDisplayFlag(a);
#if SYSCALL_LOG
			Util.Log("maNotificationPushSetDisplayFlag("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 504:
			if(mIoctls.maCaptureSetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCaptureSetProperty(a, b);
#if SYSCALL_LOG
			Util.Log("maCaptureSetProperty("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 505:
			if(mIoctls.maCaptureGetProperty == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCaptureGetProperty(a, b, c);
#if SYSCALL_LOG
			Util.Log("maCaptureGetProperty("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 506:
			if(mIoctls.maCaptureAction == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCaptureAction(a);
#if SYSCALL_LOG
			Util.Log("maCaptureAction("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 507:
			if(mIoctls.maCaptureWriteImage == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCaptureWriteImage(a, b, c);
#if SYSCALL_LOG
			Util.Log("maCaptureWriteImage("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 508:
			if(mIoctls.maCaptureGetImagePath == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCaptureGetImagePath(a, b, c);
#if SYSCALL_LOG
			Util.Log("maCaptureGetImagePath("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 509:
			if(mIoctls.maCaptureGetVideoPath == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCaptureGetVideoPath(a, b, c);
#if SYSCALL_LOG
			Util.Log("maCaptureGetVideoPath("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 510:
			if(mIoctls.maCaptureDestroyData == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maCaptureDestroyData(a);
#if SYSCALL_LOG
			Util.Log("maCaptureDestroyData("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 511:
			if(mIoctls.maPurchaseSupported == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseSupported();
#if SYSCALL_LOG
			Util.Log("maPurchaseSupported("+
				"): "+result+"\n");
#endif
		return result;
			case 512:
			if(mIoctls.maPurchaseCreate == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseCreate(a, b);
#if SYSCALL_LOG
			Util.Log("maPurchaseCreate("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 513:
			if(mIoctls.maPurchaseSetPublicKey == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseSetPublicKey(a);
#if SYSCALL_LOG
			Util.Log("maPurchaseSetPublicKey("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 514:
			if(mIoctls.maPurchaseRequest == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseRequest(a, b);
#if SYSCALL_LOG
			Util.Log("maPurchaseRequest("+
				a+
				","+b+
				"): "+result+"\n");
#endif
		return result;
			case 515:
			if(mIoctls.maPurchaseVerifyReceipt == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseVerifyReceipt(a);
#if SYSCALL_LOG
			Util.Log("maPurchaseVerifyReceipt("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 516:
			if(mIoctls.maPurchaseGetField == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseGetField(a, b, c, mCore.GetStackValue(0));
#if SYSCALL_LOG
			Util.Log("maPurchaseGetField("+
				a+
				","+b+
				","+c+
				","+mCore.GetStackValue(0)+
				"): "+result+"\n");
#endif
		return result;
			case 517:
			if(mIoctls.maPurchaseSetStoreURL == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseSetStoreURL(a);
#if SYSCALL_LOG
			Util.Log("maPurchaseSetStoreURL("+
				a+
				"): "+result+"\n");
#endif
		return result;
			case 518:
			if(mIoctls.maPurchaseGetName == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseGetName(a, b, c);
#if SYSCALL_LOG
			Util.Log("maPurchaseGetName("+
				a+
				","+b+
				","+c+
				"): "+result+"\n");
#endif
		return result;
			case 519:
			if(mIoctls.maPurchaseRestoreTransactions == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseRestoreTransactions();
#if SYSCALL_LOG
			Util.Log("maPurchaseRestoreTransactions("+
				"): "+result+"\n");
#endif
		return result;
			case 520:
			if(mIoctls.maPurchaseDestroy == null)
				result = MoSync.Constants.IOCTL_UNAVAILABLE;
			else
			result = mIoctls.maPurchaseDestroy(a);
#if SYSCALL_LOG
			Util.Log("maPurchaseDestroy("+
				a+
				"): "+result+"\n");
#endif
		return result;
		}
		return MoSync.Constants.IOCTL_UNAVAILABLE;
	}
}

} // namespace MoSync
