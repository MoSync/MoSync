MoSyncGenerated = {};

MoSyncHash = 0x778edd5f;

MoSyncConstants = {
	TRANS_NONE: 0,
	TRANS_ROT90: 5,
	TRANS_ROT180: 3,
	TRANS_ROT270: 6,
	TRANS_MIRROR: 2,
	TRANS_MIRROR_ROT90: 7,
	TRANS_MIRROR_ROT180: 1,
	TRANS_MIRROR_ROT270: 4,
	HANDLE_SCREEN: 0,
	HANDLE_LOCAL: 0,
	RES_OUT_OF_MEMORY: -1,
	RES_BAD_INPUT: -2,
	RES_OK: 1,
	MAS_CREATE_IF_NECESSARY: 1,
	STERR_GENERIC: -2,
	STERR_FULL: -3,
	STERR_NONEXISTENT: -5,
	CONNERR_GENERIC: -2,
	CONNERR_MAX: -3,
	CONNERR_DNS: -4,
	CONNERR_INTERNAL: -5,
	CONNERR_CLOSED: -6,
	CONNERR_READONLY: -7,
	CONNERR_FORBIDDEN: -8,
	CONNERR_UNINITIALIZED: -9,
	CONNERR_CONLEN: -10,
	CONNERR_URL: -11,
	CONNERR_UNAVAILABLE: -12,
	CONNERR_CANCELED: -13,
	CONNERR_PROTOCOL: -14,
	CONNERR_NETWORK: -15,
	CONNERR_NOHEADER: -16,
	CONNERR_NOTFOUND: -17,
	CONNERR_SSL: -18,
	CONNERR_USER: -1000000,
	CONNOP_READ: 1,
	CONNOP_WRITE: 2,
	CONNOP_CONNECT: 7,
	CONNOP_FINISH: 11,
	CONNOP_ACCEPT: 16,
	CONN_MAX: 32,
	BTADDR_LEN: 6,
	CONN_FAMILY_INET4: 1,
	CONN_FAMILY_BT: 2,
	HTTP_GET: 1,
	HTTP_POST: 2,
	HTTP_HEAD: 3,
	HTTP_PUT: 4,
	HTTP_DELETE: 5,
	MAK_UNKNOWN: 0,
	MAK_FIRST: 0,
	MAK_BACKSPACE: 8,
	MAK_TAB: 9,
	MAK_CLEAR: 12,
	MAK_RETURN: 13,
	MAK_PAUSE: 19,
	MAK_ESCAPE: 27,
	MAK_SPACE: 32,
	MAK_EXCLAIM: 33,
	MAK_QUOTEDBL: 34,
	MAK_POUND: 35,
	MAK_HASH: 35,
	MAK_GRID: 35,
	MAK_DOLLAR: 36,
	MAK_AMPERSAND: 38,
	MAK_QUOTE: 39,
	MAK_LEFTPAREN: 40,
	MAK_RIGHTPAREN: 41,
	MAK_ASTERISK: 42,
	MAK_STAR: 42,
	MAK_PLUS: 43,
	MAK_COMMA: 44,
	MAK_MINUS: 45,
	MAK_PERIOD: 46,
	MAK_SLASH: 47,
	MAK_0: 48,
	MAK_1: 49,
	MAK_2: 50,
	MAK_3: 51,
	MAK_4: 52,
	MAK_5: 53,
	MAK_6: 54,
	MAK_7: 55,
	MAK_8: 56,
	MAK_9: 57,
	MAK_COLON: 58,
	MAK_SEMICOLON: 59,
	MAK_LESS: 60,
	MAK_EQUALS: 61,
	MAK_GREATER: 62,
	MAK_QUESTION: 63,
	MAK_AT: 64,
	MAK_LEFTBRACKET: 91,
	MAK_BACKSLASH: 92,
	MAK_RIGHTBRACKET: 93,
	MAK_CARET: 94,
	MAK_UNDERSCORE: 95,
	MAK_BACKQUOTE: 96,
	MAK_A: 97,
	MAK_B: 98,
	MAK_C: 99,
	MAK_D: 100,
	MAK_E: 101,
	MAK_F: 102,
	MAK_G: 103,
	MAK_H: 104,
	MAK_I: 105,
	MAK_J: 106,
	MAK_K: 107,
	MAK_L: 108,
	MAK_M: 109,
	MAK_N: 110,
	MAK_O: 111,
	MAK_P: 112,
	MAK_Q: 113,
	MAK_R: 114,
	MAK_S: 115,
	MAK_T: 116,
	MAK_U: 117,
	MAK_V: 118,
	MAK_W: 119,
	MAK_X: 120,
	MAK_Y: 121,
	MAK_Z: 122,
	MAK_DELETE: 127,
	MAK_KP0: 256,
	MAK_KP1: 257,
	MAK_KP2: 258,
	MAK_KP3: 259,
	MAK_KP4: 260,
	MAK_KP5: 261,
	MAK_KP6: 262,
	MAK_KP7: 263,
	MAK_KP8: 264,
	MAK_KP9: 265,
	MAK_KP_PERIOD: 266,
	MAK_KP_DIVIDE: 267,
	MAK_KP_MULTIPLY: 268,
	MAK_KP_MINUS: 269,
	MAK_KP_PLUS: 270,
	MAK_KP_ENTER: 271,
	MAK_KP_EQUALS: 272,
	MAK_UP: 273,
	MAK_DOWN: 274,
	MAK_RIGHT: 275,
	MAK_LEFT: 276,
	MAK_INSERT: 277,
	MAK_HOME: 278,
	MAK_END: 279,
	MAK_PAGEUP: 280,
	MAK_PAGEDOWN: 281,
	MAK_FIRE: 284,
	MAK_SOFTLEFT: 285,
	MAK_SOFTRIGHT: 286,
	MAK_PEN: 291,
	MAK_BACK: 292,
	MAK_MENU: 293,
	MAK_RSHIFT: 303,
	MAK_LSHIFT: 304,
	MAK_RCTRL: 305,
	MAK_LCTRL: 306,
	MAK_RALT: 307,
	MAK_LALT: 308,
	MAK_SEARCH: 309,
	MAKB_LEFT: 0x00001,
	MAKB_UP: 0x00002,
	MAKB_RIGHT: 0x00004,
	MAKB_DOWN: 0x00008,
	MAKB_FIRE: 0x00010,
	MAKB_SOFTLEFT: 0x00020,
	MAKB_SOFTRIGHT: 0x00040,
	MAKB_0: 0x00080,
	MAKB_1: 0x00100,
	MAKB_2: 0x00200,
	MAKB_3: 0x00400,
	MAKB_4: 0x00800,
	MAKB_5: 0x01000,
	MAKB_6: 0x02000,
	MAKB_7: 0x04000,
	MAKB_8: 0x08000,
	MAKB_9: 0x10000,
	MAKB_ASTERISK: 0x20000,
	MAKB_STAR: 0x20000,
	MAKB_HASH: 0x40000,
	MAKB_POUND: 0x40000,
	MAKB_GRID: 0x40000,
	MAKB_CLEAR: 0x80000,
	EVENT_BUFFER_SIZE: 256,
	EVENT_CLOSE_TIMEOUT: 2000,
	EVENT_TYPE_CLOSE: 1,
	EVENT_TYPE_KEY_PRESSED: 2,
	EVENT_TYPE_KEY_RELEASED: 3,
	EVENT_TYPE_CONN: 4,
	EVENT_TYPE_BT: 5,
	EVENT_TYPE_TTS: 6,
	EVENT_TYPE_WLAN: 7,
	EVENT_TYPE_POINTER_PRESSED: 8,
	EVENT_TYPE_POINTER_RELEASED: 9,
	EVENT_TYPE_POINTER_DRAGGED: 10,
	EVENT_TYPE_CALL: 11,
	EVENT_TYPE_FOCUS_LOST: 13,
	EVENT_TYPE_FOCUS_GAINED: 14,
	EVENT_TYPE_STREAM: 15,
	EVENT_TYPE_LOCATION: 16,
	EVENT_TYPE_LOCATION_PROVIDER: 17,
	EVENT_TYPE_AUDIOBUFFER_FILL: 18,
	EVENT_TYPE_SCREEN_CHANGED: 21,
	EVENT_TYPE_CHAR: 22,
	EVENT_TYPE_TEXTBOX: 23,
	EVENT_TYPE_HOMESCREEN_SHOWN: 24,
	EVENT_TYPE_HOMESCREEN_HIDDEN: 25,
	EVENT_TYPE_SCREEN_STATE_ON: 26,
	EVENT_TYPE_SCREEN_STATE_OFF: 27,
	EVENT_TYPE_WIDGET: 28,
	EVENT_TYPE_BLUETOOTH_TURNED_OFF: 29,
	EVENT_TYPE_BLUETOOTH_TURNED_ON: 30,
	EVENT_TYPE_IMAGE_PICKER: 31,
	EVENT_TYPE_SMS: 32,
	EVENT_TYPE_SENSOR: 33,
	EVENT_TYPE_ALERT: 34,
	EVENT_TYPE_NFC_TAG_RECEIVED: 35,
	EVENT_TYPE_NFC_TAG_DATA_READ: 36,
	EVENT_TYPE_NFC_TAG_DATA_WRITTEN: 37,
	EVENT_TYPE_NFC_BATCH_OP: 38,
	EVENT_TYPE_NFC_TAG_AUTH_COMPLETE: 39,
	EVENT_TYPE_NFC_TAG_READ_ONLY: 40,
	EVENT_TYPE_OPTIONS_BOX_BUTTON_CLICKED: 41,
	RUNTIME_MORE: 1,
	RUNTIME_JAVA: 2,
	RUNTIME_SYMBIAN: 3,
	RUNTIME_WINCE: 4,
	REPORT_PANIC: 1,
	REPORT_EXCEPTION: 2,
	REPORT_PLATFORM_CODE: 3,
	REPORT_USER_PANIC: 4,
	REPORT_TIMEOUT: 5,
	RT_PLACEHOLDER: 1,
	RT_IMAGE: 2,
	RT_SPRITE: 3,
	RT_BINARY: 4,
	RT_UBIN: 5,
	RT_SKIP: 6,
	RT_LABEL: 9,
	RT_FLUX: 127,
	FONT_TYPE_SERIF: 0,
	FONT_TYPE_SANS_SERIF: 1,
	FONT_TYPE_MONOSPACE: 2,
	FONT_STYLE_NORMAL: 0,
	FONT_STYLE_BOLD: 1,
	FONT_STYLE_ITALIC: 2,
	RES_FONT_OK: 1,
	RES_FONT_INVALID_HANDLE: -1,
	RES_FONT_INDEX_OUT_OF_BOUNDS: -2,
	RES_FONT_NO_TYPE_STYLE_COMBINATION: -3,
	RES_FONT_NAME_NONEXISTENT: -4,
	RES_FONT_LIST_NOT_INITIALIZED: -5,
	RES_FONT_INSUFFICIENT_BUFFER: -6,
	RES_FONT_INVALID_SIZE: -7,
	RES_FONT_DELETE_DENIED: -8,
	WLAN_ADDR_LEN: 6,
	MA_LOC_NONE: 1,
	MA_LOC_INVALID: 2,
	MA_LOC_UNQUALIFIED: 3,
	MA_LOC_QUALIFIED: 4,
	MA_LPS_AVAILABLE: 1,
	MA_LPS_TEMPORARILY_UNAVAILABLE: 2,
	MA_LPS_OUT_OF_SERVICE: 3,
	MA_ACCESS_READ: 1,
	MA_ACCESS_READ_WRITE: 3,
	MA_SEEK_SET: 0,
	MA_SEEK_CUR: 1,
	MA_SEEK_END: 2,
	MA_FL_SORT_NONE: 0,
	MA_FL_SORT_DATE: 1,
	MA_FL_SORT_NAME: 2,
	MA_FL_SORT_SIZE: 3,
	MA_FL_ORDER_ASCENDING: 0x10000,
	MA_FL_ORDER_DESCENDING: 0x20000,
	MA_FERR_GENERIC: -2,
	MA_FERR_NOTFOUND: -3,
	MA_FERR_FORBIDDEN: -4,
	MA_FERR_RENAME_FILESYSTEM: -5,
	MA_FERR_RENAME_DIRECTORY: -6,
	MA_FERR_WRONG_TYPE: -7,
	MA_FERR_SORTING_UNSUPPORTED: -8,
	MA_SMS_RESULT_SENT: 1,
	MA_SMS_RESULT_NOT_SENT: 2,
	MA_SMS_RESULT_DELIVERED: 3,
	MA_SMS_RESULT_NOT_DELIVERED: 4,
	CALLSTATE_UNKNOWN: 0,
	CALLSTATE_IDLE: 1,
	CALLSTATE_DIALLING: 2,
	CALLSTATE_RINGING: 3,
	CALLSTATE_ANSWERING: 4,
	CALLSTATE_CONNECTING: 5,
	CALLSTATE_CONNECTED: 6,
	CALLSTATE_RECONNECTPENDING: 7,
	CALLSTATE_DISCONNECTING: 8,
	CALLSTATE_HOLD: 9,
	CALLSTATE_TRANSFERRING: 10,
	CALLSTATE_TRANSFERALERTING: 11,
	STREAMEVENT_OPEN_COMPLETE: 1,
	STREAMEVENT_PREPARE_COMPLETE: 2,
	STREAMEVENT_PLAY_COMPLETE: 3,
	STREAMEVENT_LOADING_STARTED: 4,
	STREAMEVENT_LOADING_COMPLETE: 5,
	STREAMEVENT_UNKNOWN_EVENT: 6,
	AUDIO_FMT_S8: 1,
	AUDIO_FMT_S16: 2,
	AUDIO_FMT_U8: 3,
	AUDIO_FMT_U16: 4,
	MA_PIM_CONTACTS: 1,
	MA_PIM_EVENTS: 2,
	MA_PIM_TODOS: 3,
	MA_PIM_TYPE_BINARY: 0,
	MA_PIM_TYPE_BOOLEAN: 1,
	MA_PIM_TYPE_DATE: 2,
	MA_PIM_TYPE_INT: 3,
	MA_PIM_TYPE_STRING: 4,
	MA_PIM_TYPE_STRING_ARRAY: 5,
	MA_PIM_FIELD_CONTACT_ADDR: 100,
	MA_PIM_FIELD_CONTACT_BIRTHDAY: 101,
	MA_PIM_FIELD_CONTACT_CLASS: 102,
	MA_PIM_FIELD_CONTACT_EMAIL: 103,
	MA_PIM_FIELD_CONTACT_FORMATTED_ADDR: 104,
	MA_PIM_FIELD_CONTACT_FORMATTED_NAME: 105,
	MA_PIM_FIELD_CONTACT_NAME: 106,
	MA_PIM_FIELD_CONTACT_NICKNAME: 107,
	MA_PIM_FIELD_CONTACT_NOTE: 108,
	MA_PIM_FIELD_CONTACT_ORG: 109,
	MA_PIM_FIELD_CONTACT_PHOTO: 110,
	MA_PIM_FIELD_CONTACT_PHOTO_URL: 111,
	MA_PIM_FIELD_CONTACT_PUBLIC_KEY: 112,
	MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING: 113,
	MA_PIM_FIELD_CONTACT_REVISION: 114,
	MA_PIM_FIELD_CONTACT_TEL: 115,
	MA_PIM_FIELD_CONTACT_TITLE: 116,
	MA_PIM_FIELD_CONTACT_UID: 117,
	MA_PIM_FIELD_CONTACT_URL: 118,
	MA_PIM_FIELD_CONTACT_IM: 119,
	MA_PIM_FIELD_CONTACT_RELATION: 120,
	MA_PIM_FIELD_CONTACT_ORG_INFO: 121,
	MA_PIM_CONTACT_ADDR_POBOX: 0,
	MA_PIM_CONTACT_ADDR_EXTRA: 1,
	MA_PIM_CONTACT_ADDR_STREET: 2,
	MA_PIM_CONTACT_ADDR_LOCALITY: 3,
	MA_PIM_CONTACT_ADDR_REGION: 4,
	MA_PIM_CONTACT_ADDR_POSTALCODE: 5,
	MA_PIM_CONTACT_ADDR_COUNTRY: 6,
	MA_PIM_CONTACT_ADDR_NEIGHBORHOOD: 7,
	MA_PIM_CONTACT_NAME_FAMILY: 0,
	MA_PIM_CONTACT_NAME_GIVEN: 1,
	MA_PIM_CONTACT_NAME_OTHER: 2,
	MA_PIM_CONTACT_NAME_PREFIX: 3,
	MA_PIM_CONTACT_NAME_SUFFIX: 4,
	MA_PIM_CONTACT_NAME_PHONETIC_FAMILY: 5,
	MA_PIM_CONTACT_NAME_PHONETIC_GIVEN: 6,
	MA_PIM_CONTACT_NAME_PHONETIC_OTHER: 7,
	MA_PIM_CONTACT_ORG_INFO_DEPARTMENT: 0,
	MA_PIM_CONTACT_ORG_INFO_JOB_DESCRIPTION: 1,
	MA_PIM_CONTACT_ORG_INFO_SYMBOL: 2,
	MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME: 3,
	MA_PIM_CONTACT_ORG_INFO_OFFICE_LOCATION: 4,
	MA_PIM_CONTACT_ORG_INFO_PHONETIC_NAME_STYLE: 5,
	MA_PIM_CONTACT_IM_USERNAME: 0,
	MA_PIM_CONTACT_IM_PROTOCOL: 1,
	MA_PIM_CONTACT_CLASS_CONFIDENTIAL: 200,
	MA_PIM_CONTACT_CLASS_PRIVATE: 201,
	MA_PIM_CONTACT_CLASS_PUBLIC: 202,
	MA_PIM_CONTACT_IM_PROTOCOL_AIM: "AIM",
	MA_PIM_CONTACT_IM_PROTOCOL_MSN: "MSN",
	MA_PIM_CONTACT_IM_PROTOCOL_YAHOO: "Yahoo",
	MA_PIM_CONTACT_IM_PROTOCOL_SKYPE: "Skype",
	MA_PIM_CONTACT_IM_PROTOCOL_QQ: "QQ",
	MA_PIM_CONTACT_IM_PROTOCOL_GOOGLE_TALK: "GoogleTalk",
	MA_PIM_CONTACT_IM_PROTOCOL_ICQ: "ICQ",
	MA_PIM_CONTACT_IM_PROTOCOL_JABBER: "Jabber",
	MA_PIM_CONTACT_IM_PROTOCOL_NETMEETING: "Netmeeting",
	MA_PIM_ATTR_ASST: 1,
	MA_PIM_ATTR_AUTO: 2,
	MA_PIM_ATTR_FAX: 4,
	MA_PIM_ATTR_HOME: 8,
	MA_PIM_ATTR_MOBILE: 16,
	MA_PIM_ATTR_OTHER: 32,
	MA_PIM_ATTR_PAGER: 64,
	MA_PIM_ATTR_PREFERRED: 128,
	MA_PIM_ATTR_SMS: 256,
	MA_PIM_ATTR_WORK: 512,
	MA_PIM_ATTR_ADDR_HOME: 101,
	MA_PIM_ATTR_ADDR_WORK: 102,
	MA_PIM_ATTR_ADDR_OTHER: 103,
	MA_PIM_ATTR_ADDR_CUSTOM: 104,
	MA_PIM_ATTR_EMAIL_HOME: 201,
	MA_PIM_ATTR_EMAIL_WORK: 202,
	MA_PIM_ATTR_EMAIL_MOBILE: 203,
	MA_PIM_ATTR_EMAIL_OTHER: 204,
	MA_PIM_ATTR_EMAIL_CUSTOM: 205,
	MA_PIM_ATTR_FORMATTED_ADDR_HOME: 301,
	MA_PIM_ATTR_FORMATTED_ADDR_WORK: 302,
	MA_PIM_ATTR_FORMATTED_ADDR_OTHER: 303,
	MA_PIM_ATTR_FORMATTED_ADDR_CUSTOM: 304,
	MA_PIM_ATTR_ORG_WORK: 401,
	MA_PIM_ATTR_ORG_OTHER: 402,
	MA_PIM_ATTR_ORG_CUSTOM: 403,
	MA_PIM_ATTR_PHONE_HOME: 501,
	MA_PIM_ATTR_PHONE_MOBILE: 502,
	MA_PIM_ATTR_PHONE_HOME_FAX: 503,
	MA_PIM_ATTR_PHONE_WORK_FAX: 504,
	MA_PIM_ATTR_PHONE_PAGER: 505,
	MA_PIM_ATTR_PHONE_IPHONE: 506,
	MA_PIM_ATTR_PHONE_WORK: 507,
	MA_PIM_ATTR_PHONE_CALLBACK: 508,
	MA_PIM_ATTR_PHONE_CAR: 509,
	MA_PIM_ATTR_PHONE_COMPANY_MAIN: 510,
	MA_PIM_ATTR_PHONE_ISDN: 511,
	MA_PIM_ATTR_PHONE_OTHER_FAX: 512,
	MA_PIM_ATTR_PHONE_RADIO: 513,
	MA_PIM_ATTR_PHONE_TELEX: 514,
	MA_PIM_ATTR_PHONE_TTY_TDD: 515,
	MA_PIM_ATTR_PHONE_WORK_MOBILE: 516,
	MA_PIM_ATTR_PHONE_WORK_PAGER: 517,
	MA_PIM_ATTR_PHONE_ASSISTANT: 518,
	MA_PIM_ATTR_PHONE_MMS: 519,
	MA_PIM_ATTR_PHONE_OTHER: 520,
	MA_PIM_ATTR_PHONE_CUSTOM: 521,
	MA_PIM_ATTR_TITLE_WORK: 601,
	MA_PIM_ATTR_TITLE_OTHER: 602,
	MA_PIM_ATTR_TITLE_CUSTOM: 603,
	MA_PIM_ATTR_WEBSITE_HOMEPAGE: 701,
	MA_PIM_ATTR_WEBSITE_BLOG: 702,
	MA_PIM_ATTR_WEBSITE_PROFILE: 703,
	MA_PIM_ATTR_WEBSITE_HOME: 704,
	MA_PIM_ATTR_WEBSITE_WORK: 705,
	MA_PIM_ATTR_WEBSITE_FTP: 706,
	MA_PIM_ATTR_WEBSITE_OTHER: 707,
	MA_PIM_ATTR_WEBSITE_CUSTOM: 708,
	MA_PIM_ATTR_IM_HOME: 801,
	MA_PIM_ATTR_IM_WORK: 802,
	MA_PIM_ATTR_IM_OTHER: 803,
	MA_PIM_ATTR_IM_CUSTOM: 804,
	MA_PIM_ATTR_RELATION_MOTHER: 901,
	MA_PIM_ATTR_RELATION_FATHER: 902,
	MA_PIM_ATTR_RELATION_PARENT: 903,
	MA_PIM_ATTR_RELATION_SISTER: 904,
	MA_PIM_ATTR_RELATION_BROTHER: 905,
	MA_PIM_ATTR_RELATION_CHILD: 906,
	MA_PIM_ATTR_RELATION_FRIEND: 907,
	MA_PIM_ATTR_RELATION_SPOUSE: 908,
	MA_PIM_ATTR_RELATION_PARTNER: 909,
	MA_PIM_ATTR_RELATION_MANAGER: 910,
	MA_PIM_ATTR_RELATION_ASSISTANT: 911,
	MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER: 912,
	MA_PIM_ATTR_RELATION_REFERRED_BY: 913,
	MA_PIM_ATTR_RELATION_RELATIVE: 914,
	MA_PIM_ATTR_RELATION_CUSTOM: 915,
	MA_PIM_ATTR_ORG_INFO_WORK: 1001,
	MA_PIM_ATTR_ORG_INFO_OTHER: 1002,
	MA_PIM_ATTR_ORG_INFO_CUSTOM: 1003,
	MA_PIM_ATTRPREFERRED: 0x10000,
	MA_PIM_ERR_NONE: 0,
	MA_PIM_ERR_UNAVAILABLE: -1,
	MA_PIM_ERR_NATIVE_TYPE_MISMATCH: -2,
	MA_PIM_ERR_LIST_TYPE_INVALID: -3,
	MA_PIM_ERR_LIST_UNAVAILABLE: -4,
	MA_PIM_ERR_LIST_ALREADY_OPENED: -5,
	MA_PIM_ERR_FIELD_INVALID: -6,
	MA_PIM_ERR_FIELD_UNSUPPORTED: -7,
	MA_PIM_ERR_FIELD_EMPTY: -8,
	MA_PIM_ERR_FIELD_COUNT_MAX: -9,
	MA_PIM_ERR_FIELD_READ_ONLY: -10,
	MA_PIM_ERR_FIELD_WRITE_ONLY: -11,
	MA_PIM_ERR_INDEX_INVALID: -12,
	MA_PIM_ERR_HANDLE_INVALID: -13,
	MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED: -14,
	MA_PIM_ERR_ATTRIBUTE_CUSTOM: -15,
	MA_PIM_ERR_BUFFER_INVALID: -16,
	MA_PIM_ERR_NO_LABEL: -17,
	MA_PIM_ERR_OPERATION_NOT_PERMITTED: -18,
	MAW_EVENT_POINTER_PRESSED: 2,
	MAW_EVENT_POINTER_RELEASED: 3,
	MAW_EVENT_CONTENT_LOADED: 4,
	MAW_EVENT_CLICKED: 5,
	MAW_EVENT_ITEM_CLICKED: 6,
	MAW_EVENT_TAB_CHANGED: 7,
	MAW_EVENT_GL_VIEW_READY: 8,
	MAW_EVENT_WEB_VIEW_URL_CHANGED: 9,
	MAW_EVENT_STACK_SCREEN_POPPED: 10,
	MAW_EVENT_SLIDER_VALUE_CHANGED: 11,
	MAW_EVENT_DATE_PICKER_VALUE_CHANGED: 12,
	MAW_EVENT_TIME_PICKER_VALUE_CHANGED: 13,
	MAW_EVENT_NUMBER_PICKER_VALUE_CHANGED: 14,
	MAW_EVENT_VIDEO_STATE_CHANGED: 15,
	MAW_EVENT_EDIT_BOX_EDITING_DID_BEGIN: 16,
	MAW_EVENT_EDIT_BOX_EDITING_DID_END: 17,
	MAW_EVENT_EDIT_BOX_TEXT_CHANGED: 18,
	MAW_EVENT_EDIT_BOX_RETURN: 19,
	MAW_EVENT_WEB_VIEW_CONTENT_LOADING: 20,
	MAW_EVENT_WEB_VIEW_HOOK_INVOKED: 21,
	MAW_EVENT_DIALOG_DISMISSED: 22,
	MAW_CONSTANT_MOSYNC_SCREEN_HANDLE: 0,
	MAW_CONSTANT_FILL_AVAILABLE_SPACE: -1,
	MAW_CONSTANT_WRAP_CONTENT: -2,
	MAW_CONSTANT_STARTED: 1,
	MAW_CONSTANT_DONE: 2,
	MAW_CONSTANT_STOPPED: 3,
	MAW_CONSTANT_ERROR: -1,
	MAW_CONSTANT_SOFT: 5,
	MAW_CONSTANT_HARD: 6,
	MAW_CONSTANT_ARROW_UP: 1,
	MAW_CONSTANT_ARROW_DOWN: 2,
	MAW_CONSTANT_ARROW_LEFT: 4,
	MAW_CONSTANT_ARROW_RIGHT: 8,
	MAW_CONSTANT_ARROW_ANY: 15,
	MAW_ALIGNMENT_LEFT: "left",
	MAW_ALIGNMENT_RIGHT: "right",
	MAW_ALIGNMENT_CENTER: "center",
	MAW_ALIGNMENT_TOP: "top",
	MAW_ALIGNMENT_BOTTOM: "bottom",
	MAW_VIDEO_VIEW_ACTION_PLAY: 1,
	MAW_VIDEO_VIEW_ACTION_PAUSE: 2,
	MAW_VIDEO_VIEW_ACTION_STOP: 3,
	MAW_VIDEO_VIEW_STATE_PLAYING: 1,
	MAW_VIDEO_VIEW_STATE_PAUSED: 2,
	MAW_VIDEO_VIEW_STATE_STOPPED: 3,
	MAW_VIDEO_VIEW_STATE_FINISHED: 4,
	MAW_VIDEO_VIEW_STATE_SOURCE_READY: 5,
	MAW_VIDEO_VIEW_STATE_INTERRUPTED: 6,
	MAW_RES_OK: 0,
	MAW_RES_ERROR: -2,
	MAW_RES_INVALID_PROPERTY_NAME: -2,
	MAW_RES_INVALID_PROPERTY_VALUE: -3,
	MAW_RES_INVALID_HANDLE: -4,
	MAW_RES_INVALID_TYPE_NAME: -5,
	MAW_RES_INVALID_INDEX: -6,
	MAW_RES_INVALID_STRING_BUFFER_SIZE: -7,
	MAW_RES_INVALID_SCREEN: -8,
	MAW_RES_INVALID_LAYOUT: -9,
	MAW_RES_REMOVED_ROOT: -10,
	MAW_RES_FEATURE_NOT_AVAILABLE: -11,
	MAW_RES_CANNOT_INSERT_DIALOG: -12,
	MAW_SCREEN: "Screen",
	MAW_TAB_SCREEN: "TabScreen",
	MAW_STACK_SCREEN: "StackScreen",
	MAW_BUTTON: "Button",
	MAW_IMAGE: "Image",
	MAW_IMAGE_BUTTON: "ImageButton",
	MAW_LABEL: "Label",
	MAW_EDIT_BOX: "EditBox",
	MAW_LIST_VIEW: "ListView",
	MAW_LIST_VIEW_ITEM: "ListViewItem",
	MAW_CHECK_BOX: "CheckBox",
	MAW_HORIZONTAL_LAYOUT: "HorizontalLayout",
	MAW_VERTICAL_LAYOUT: "VerticalLayout",
	MAW_RELATIVE_LAYOUT: "RelativeLayout",
	MAW_SEARCH_BAR: "SearchBar",
	MAW_NAV_BAR: "NavBar",
	MAW_GL_VIEW: "GLView",
	MAW_GL2_VIEW: "GL2View",
	MAW_CAMERA_PREVIEW: "CameraPreview",
	MAW_WEB_VIEW: "WebView",
	MAW_PROGRESS_BAR: "ProgressBar",
	MAW_ACTIVITY_INDICATOR: "ActivityIndicator",
	MAW_SLIDER: "Slider",
	MAW_DATE_PICKER: "DatePicker",
	MAW_TIME_PICKER: "TimePicker",
	MAW_NUMBER_PICKER: "NumberPicker",
	MAW_VIDEO_VIEW: "VideoView",
	MAW_TOGGLE_BUTTON: "ToggleButton",
	MAW_MODAL_DIALOG: "ModalDialog",
	MAW_WIDGET_LEFT: "left",
	MAW_WIDGET_TOP: "top",
	MAW_WIDGET_WIDTH: "width",
	MAW_WIDGET_HEIGHT: "height",
	MAW_WIDGET_ALPHA: "alpha",
	MAW_WIDGET_BACKGROUND_COLOR: "backgroundColor",
	MAW_WIDGET_VISIBLE: "visible",
	MAW_WIDGET_ENABLED: "enabled",
	MAW_WIDGET_BACKGROUND_GRADIENT: "backgroundGradient",
	MAW_SCREEN_TITLE: "title",
	MAW_SCREEN_ICON: "icon",
	MAW_TAB_SCREEN_TITLE: "title",
	MAW_TAB_SCREEN_ICON: "icon",
	MAW_TAB_SCREEN_CURRENT_TAB: "currentTab",
	MAW_STACK_SCREEN_TITLE: "title",
	MAW_STACK_SCREEN_ICON: "icon",
	MAW_STACK_SCREEN_BACK_BUTTON_ENABLED: "backButtonEnabled",
	MAW_LABEL_TEXT: "text",
	MAW_LABEL_TEXT_VERTICAL_ALIGNMENT: "textVerticalAlignment",
	MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT: "textHorizontalAlignment",
	MAW_LABEL_FONT_COLOR: "fontColor",
	MAW_LABEL_FONT_SIZE: "fontSize",
	MAW_LABEL_FONT_HANDLE: "fontHandle",
	MAW_LABEL_MAX_NUMBER_OF_LINES: "maxNumberOfLines",
	MAW_BUTTON_TEXT: "text",
	MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT: "textVerticalAlignment",
	MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT: "textHorizontalAlignment",
	MAW_BUTTON_FONT_COLOR: "fontColor",
	MAW_BUTTON_FONT_SIZE: "fontSize",
	MAW_BUTTON_FONT_HANDLE: "fontHandle",
	MAW_IMAGE_BUTTON_TEXT: "text",
	MAW_IMAGE_BUTTON_TEXT_VERTICAL_ALIGNMENT: "textVerticalAlignment",
	MAW_IMAGE_BUTTON_TEXT_HORIZONTAL_ALIGNMENT: "textHorizontalAlignment",
	MAW_IMAGE_BUTTON_FONT_COLOR: "fontColor",
	MAW_IMAGE_BUTTON_FONT_SIZE: "fontSize",
	MAW_IMAGE_BUTTON_BACKGROUND_IMAGE: "backgroundImage",
	MAW_IMAGE_BUTTON_IMAGE: "image",
	MAW_IMAGE_BUTTON_FONT_HANDLE: "fontHandle",
	MAW_IMAGE_IMAGE: "image",
	MAW_IMAGE_SCALE_MODE: "scaleMode",
	MAW_EDIT_BOX_TYPE_ANY: 0,
	MAW_EDIT_BOX_TYPE_EMAILADDR: 1,
	MAW_EDIT_BOX_TYPE_NUMERIC: 2,
	MAW_EDIT_BOX_TYPE_PHONENUMBER: 3,
	MAW_EDIT_BOX_TYPE_URL: 4,
	MAW_EDIT_BOX_TYPE_DECIMAL: 5,
	MAW_EDIT_BOX_TYPE_SINGLE_LINE: 6,
	MAW_EDIT_BOX_FLAG_PASSWORD: 0,
	MAW_EDIT_BOX_FLAG_SENSITIVE: 1,
	MAW_EDIT_BOX_FLAG_INITIAL_CAPS_WORD: 2,
	MAW_EDIT_BOX_FLAG_INITIAL_CAPS_SENTENCE: 3,
	MAW_EDIT_BOX_FLAG_INITIAL_CAPS_ALL_CHARACTERS: 4,
	MAW_EDIT_BOX_TEXT: "text",
	MAW_EDIT_BOX_PLACEHOLDER: "placeholder",
	MAW_EDIT_BOX_SHOW_KEYBOARD: "showKeyboard",
	MAW_EDIT_BOX_EDIT_MODE: "editMode",
	MAW_EDIT_BOX_INPUT_MODE: "inputMode",
	MAW_EDIT_BOX_INPUT_FLAG: "inputFlag",
	MAW_LIST_VIEW_ITEM_TEXT: "text",
	MAW_LIST_VIEW_ITEM_ICON: "icon",
	MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE: "accessoryType",
	MAW_LIST_VIEW_ITEM_FONT_COLOR: "fontColor",
	MAW_LIST_VIEW_ITEM_FONT_SIZE: "fontSize",
	MAW_LIST_VIEW_ITEM_FONT_HANDLE: "fontHandle",
	MAW_CHECK_BOX_CHECKED: "checked",
	MAW_TOGGLE_BUTTON_CHECKED: "checked",
	MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT: "childVerticalAlignment",
	MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT: "childHorizontalAlignment",
	MAW_HORIZONTAL_LAYOUT_PADDING_TOP: "paddingTop",
	MAW_HORIZONTAL_LAYOUT_PADDING_LEFT: "paddingLeft",
	MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT: "paddingRight",
	MAW_HORIZONTAL_LAYOUT_PADDING_BOTTOM: "paddingBottom",
	MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT: "childVerticalAlignment",
	MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT: "childHorizontalAlignment",
	MAW_VERTICAL_LAYOUT_PADDING_TOP: "paddingTop",
	MAW_VERTICAL_LAYOUT_PADDING_LEFT: "paddingLeft",
	MAW_VERTICAL_LAYOUT_PADDING_RIGHT: "paddingRight",
	MAW_VERTICAL_LAYOUT_PADDING_BOTTOM: "paddingBottom",
	MAW_SEARCH_BAR_TEXT: "text",
	MAW_SEARCH_BAR_PLACEHOLDER: "placeholder",
	MAW_SEARCH_BAR_SHOW_KEYBOARD: "showKeyboard",
	MAW_GL_VIEW_INVALIDATE: "invalidate",
	MAW_GL_VIEW_BIND: "bind",
	MAW_WEB_VIEW_URL: "url",
	MAW_WEB_VIEW_HTML: "html",
	MAW_WEB_VIEW_BASE_URL: "baseUrl",
	MAW_WEB_VIEW_SOFT_HOOK: "softHook",
	MAW_WEB_VIEW_HARD_HOOK: "hardHook",
	MAW_WEB_VIEW_NEW_URL: "newurl",
	MAW_WEB_VIEW_HORIZONTAL_SCROLL_BAR_ENABLED: "horizontalScrollBarEnabled",
	MAW_WEB_VIEW_VERTICAL_SCROLL_BAR_ENABLED: "verticalScrollBarEnabled",
	MAW_WEB_VIEW_ENABLE_ZOOM: "enableZoom",
	MAW_WEB_VIEW_NAVIGATE: "navigate",
	MAW_PROGRESS_BAR_MAX: "max",
	MAW_PROGRESS_BAR_PROGRESS: "progress",
	MAW_PROGRESS_BAR_INCREMENT_PROGRESS: "incrementProgress",
	MAW_ACTIVITY_INDICATOR_IN_PROGRESS: "inProgress",
	MAW_SLIDER_MAX: "max",
	MAW_SLIDER_VALUE: "value",
	MAW_SLIDER_INCREASE_VALUE: "increaseValue",
	MAW_SLIDER_DECREASE_VALUE: "decreaseValue",
	MAW_DATE_PICKER_MAX_DATE: "maxDate",
	MAW_DATE_PICKER_MIN_DATE: "minDate",
	MAW_DATE_PICKER_YEAR: "year",
	MAW_DATE_PICKER_MONTH: "month",
	MAW_DATE_PICKER_DAY_OF_MONTH: "dayOfMonth",
	MAW_TIME_PICKER_CURRENT_HOUR: "currentHour",
	MAW_TIME_PICKER_CURRENT_MINUTE: "currentMinute",
	MAW_NUMBER_PICKER_VALUE: "value",
	MAW_NUMBER_PICKER_MIN_VALUE: "minValue",
	MAW_NUMBER_PICKER_MAX_VALUE: "maxValue",
	MAW_VIDEO_VIEW_PATH: "path",
	MAW_VIDEO_VIEW_URL: "url",
	MAW_VIDEO_VIEW_ACTION: "action",
	MAW_VIDEO_VIEW_SEEK_TO: "seekTo",
	MAW_VIDEO_VIEW_DURATION: "duration",
	MAW_VIDEO_VIEW_BUFFER_PERCENTAGE: "bufferPercentage",
	MAW_VIDEO_VIEW_CURRENT_POSITION: "currentPosition",
	MAW_NAV_BAR_TITLE: "title",
	MAW_NAV_BAR_ICON: "icon",
	MAW_NAV_BAR_BACK_BTN: "backBtn",
	MAW_NAV_BAR_TITLE_FONT_COLOR: "titleFontColor",
	MAW_NAV_BAR_TITLE_FONT_SIZE: "titleFontSize",
	MAW_NAV_BAR_TITLE_FONT_HANDLE: "titleFontHandle",
	MAW_MODAL_DIALOG_TITLE: "title",
	MAW_MODAL_DIALOG_ARROW_POSITION: "arrowPosition",
	MAW_MODAL_DIALOG_USER_CAN_DISMISS: "userCanDismiss",
	MA_CAMERA_CONST_BACK_CAMERA: 0,
	MA_CAMERA_CONST_FRONT_CAMERA: 1,
	MA_CAMERA_RES_OK: 1,
	MA_CAMERA_RES_FAILED: -2,
	MA_CAMERA_RES_NOT_STARTED: -3,
	MA_CAMERA_RES_PROPERTY_NOTSUPPORTED: -4,
	MA_CAMERA_RES_INVALID_PROPERTY_VALUE: -5,
	MA_CAMERA_RES_VALUE_NOTSUPPORTED: -6,
	MA_CAMERA_FLASH_ON: "on",
	MA_CAMERA_FLASH_AUTO: "auto",
	MA_CAMERA_FLASH_OFF: "off",
	MA_CAMERA_FLASH_TORCH: "torch",
	MA_CAMERA_FOCUS_AUTO: "auto",
	MA_CAMERA_FOCUS_INFINITY: "infinity",
	MA_CAMERA_FOCUS_MACRO: "macro",
	MA_CAMERA_FOCUS_FIXED: "fixed",
	MA_CAMERA_IMAGE_JPEG: "jpeg",
	MA_CAMERA_IMAGE_RAW: "raw",
	MA_CAMERA_FLASH_MODE: "flash-mode",
	MA_CAMERA_FOCUS_MODE: "focus-mode",
	MA_CAMERA_IMAGE_FORMAT: "image-format",
	MA_CAMERA_ZOOM: "zoom",
	MA_CAMERA_MAX_ZOOM: "max-zoom",
	MA_CAMERA_ZOOM_SUPPORTED: "zoom-supported",
	MA_CAMERA_FLASH_SUPPORTED: "flash-supported",
	MA_TB_TYPE_ANY: 0,
	MA_TB_TYPE_EMAILADDR: 1,
	MA_TB_TYPE_NUMERIC: 2,
	MA_TB_TYPE_PHONENUMBER: 3,
	MA_TB_TYPE_URL: 4,
	MA_TB_TYPE_DECIMAL: 5,
	MA_TB_TYPE_SINGLE_LINE: 100,
	MA_TB_TYPE_MASK: 0xFFFF,
	MA_TB_RES_OK: 1,
	MA_TB_RES_CANCEL: 2,
	MA_TB_RES_TYPE_UNAVAILABLE: -3,
	MA_TB_FLAG_PASSWORD: 0x10000,
	MA_TB_FLAG_UNEDITABLE: 0x20000,
	MA_TB_FLAG_SENSITIVE: 0x40000,
	MA_TB_FLAG_NON_PREDICTIVE: 0x80000,
	MA_TB_FLAG_INITIAL_CAPS_WORD: 0x100000,
	MA_TB_FLAG_INITIAL_CAPS_SENTENCE: 0x200000,
	NOTIFICATION_TYPE_APPLICATION_LAUNCHER: 1,
	SCREEN_ORIENTATION_LANDSCAPE: 1,
	SCREEN_ORIENTATION_PORTRAIT: 2,
	SCREEN_ORIENTATION_DYNAMIC: 3,
	MA_GL_TEX_IMAGE_2D_OK: 0,
	MA_GL_TEX_IMAGE_2D_INVALID_IMAGE: -2,
	MA_GL_API_GL2: 0,
	MA_GL_API_GL1: 1,
	MA_GL_INIT_RES_OK: 0,
	MA_GL_INIT_RES_UNAVAILABLE_API: -2,
	MA_GL_INIT_RES_ERROR: -3,
	SENSOR_TYPE_ACCELEROMETER: 1,
	SENSOR_TYPE_MAGNETIC_FIELD: 2,
	SENSOR_TYPE_ORIENTATION: 3,
	SENSOR_TYPE_GYROSCOPE: 4,
	SENSOR_TYPE_PROXIMITY: 5,
	SENSOR_RATE_FASTEST: 0,
	SENSOR_RATE_GAME: -1,
	SENSOR_RATE_NORMAL: -2,
	SENSOR_RATE_UI: -3,
	SENSOR_ERROR_NONE: 0,
	SENSOR_ERROR_NOT_AVAILABLE: -1,
	SENSOR_ERROR_INTERVAL_NOT_SET: -2,
	SENSOR_ERROR_ALREADY_ENABLED: -3,
	SENSOR_ERROR_NOT_ENABLED: -4,
	SENSOR_ERROR_CANNOT_DISABLE: -5,
	UIDEVICE_ORIENTATION_UNKNOWN: 0,
	UIDEVICE_ORIENTATION_PORTRAIT: 1,
	UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN: 2,
	UIDEVICE_ORIENTATION_LANDSCAPE_LEFT: 3,
	UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT: 4,
	UIDEVICE_ORIENTATION_FACE_UP: 5,
	UIDEVICE_ORIENTATION_FACE_DOWN: 6,
	SENSOR_PROXIMITY_VALUE_FAR: 0,
	SENSOR_PROXIMITY_VALUE_NEAR: 1,
	MA_NFC_NOT_AVAILABLE: -1,
	MA_NFC_NOT_ENABLED: -2,
	MA_NFC_INVALID_TAG_TYPE: -2,
	MA_NFC_TAG_CONNECTION_LOST: -3,
	MA_NFC_TAG_NOT_CONNECTED: -4,
	MA_NFC_FORMAT_FAILED: -5,
	MA_NFC_TAG_IO_ERROR: -127,
	MA_NFC_TAG_TYPE_NDEF: 1,
	MA_NFC_TAG_TYPE_MIFARE_CL: 2,
	MA_NFC_TAG_TYPE_MIFARE_UL: 3,
	MA_NFC_TAG_TYPE_NFC_A: 4,
	MA_NFC_TAG_TYPE_NFC_B: 5,
	MA_NFC_TAG_TYPE_ISO_DEP: 6,
	MA_NFC_TAG_TYPE_NDEF_FORMATTABLE: 128,
	MA_NFC_NDEF_TNF_EMPTY: 0,
	MA_NFC_NDEF_TNF_WELL_KNOWN: 1,
	MA_NFC_NDEF_TNF_MIME_MEDIA: 2,
	MA_NFC_NDEF_TNF_ABSOLUTE_URI: 3,
	MA_NFC_NDEF_TNF_EXTERNAL_TYPE: 4,
	MA_NFC_NDEF_TNF_UNKNOWN: 5,
	MA_NFC_NDEF_TNF_UNCHANGED: 6,
	MA_NFC_NDEF_TNF_RESERVED: 7,
	MA_NFC_MIFARE_KEY_A: 1,
	MA_NFC_MIFARE_KEY_B: 2,
	IOCTL_UNAVAILABLE: -1
};
MoSyncGenerated.invokeSyscall = function(id) {
	switch(id) {
		case 1:
			ret = this.Syscalls.maCheckInterfaceVersion(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 2:
			ret = this.Syscalls.maExit(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 3:
			ret = this.Syscalls.maPanic(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 4:
			ret = this.Syscalls.memset(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 5:
			ret = this.Syscalls.memcpy(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 6:
			ret = this.Syscalls.strcmp(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 7:
			ret = this.Syscalls.strcpy(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 8:
			ret = this.Syscalls.__adddf3(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 9:
			ret = this.Syscalls.__subdf3(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 10:
			ret = this.Syscalls.__muldf3(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 11:
			ret = this.Syscalls.__divdf3(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 12:
			ret = this.Syscalls.__negdf2(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 13:
			ret = this.Syscalls.__fixdfsi(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 14:
			ret = this.Syscalls.__fixunsdfsi(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 15:
			ret = this.Syscalls.__floatsidf(this.regs[Reg.i0]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 16:
			ret = this.Syscalls.__extendsfdf2(this.regs[Reg.i0]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 17:
			ret = this.Syscalls.dcmp(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 18:
			ret = this.Syscalls.__addsf3(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 19:
			ret = this.Syscalls.__subsf3(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 20:
			ret = this.Syscalls.__mulsf3(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 21:
			ret = this.Syscalls.__divsf3(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 22:
			ret = this.Syscalls.__negsf2(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 23:
			ret = this.Syscalls.__fixsfsi(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 24:
			ret = this.Syscalls.__fixunssfsi(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 25:
			ret = this.Syscalls.__floatsisf(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 26:
			ret = this.Syscalls.__truncdfsf2(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 27:
			ret = this.Syscalls.fcmp(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 28:
			ret = this.Syscalls.sin(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 29:
			ret = this.Syscalls.cos(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 30:
			ret = this.Syscalls.tan(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 31:
			ret = this.Syscalls.sqrt(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = ret.hi;
			this.regs[Reg.r15] = ret.lo;
		break;
		case 32:
			ret = this.Syscalls.maSetColor(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 33:
			ret = this.Syscalls.maSetClipRect(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 34:
			ret = this.Syscalls.maGetClipRect(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 35:
			ret = this.Syscalls.maPlot(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 36:
			ret = this.Syscalls.maLine(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 37:
			ret = this.Syscalls.maFillRect(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 38:
			ret = this.Syscalls.maFillTriangleStrip(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 39:
			ret = this.Syscalls.maFillTriangleFan(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 40:
			ret = this.Syscalls.maGetTextSize(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 41:
			ret = this.Syscalls.maGetTextSizeW(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 42:
			ret = this.Syscalls.maDrawText(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 43:
			ret = this.Syscalls.maDrawTextW(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 44:
			ret = this.Syscalls.maUpdateScreen();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 45:
			ret = this.Syscalls.maResetBacklight();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 46:
			ret = this.Syscalls.maGetScrSize();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 47:
			ret = this.Syscalls.maDrawImage(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 48:
			ret = this.Syscalls.maDrawRGB(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 49:
			ret = this.Syscalls.maDrawImageRegion(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 50:
			ret = this.Syscalls.maGetImageSize(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 51:
			ret = this.Syscalls.maGetImageData(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 52:
			ret = this.Syscalls.maSetDrawTarget(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 53:
			ret = this.Syscalls.maFindLabel(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 54:
			ret = this.Syscalls.maCreateImageFromData(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 55:
			ret = this.Syscalls.maCreateImageRaw(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 56:
			ret = this.Syscalls.maCreateDrawableImage(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 57:
			ret = this.Syscalls.maCreateData(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 58:
			ret = this.Syscalls.maCreatePlaceholder();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 59:
			ret = this.Syscalls.maDestroyObject(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 60:
			ret = this.Syscalls.maGetDataSize(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 61:
			ret = this.Syscalls.maReadData(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 62:
			ret = this.Syscalls.maWriteData(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 63:
			ret = this.Syscalls.maCopyData(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 64:
			ret = this.Syscalls.maOpenStore(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 65:
			ret = this.Syscalls.maWriteStore(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 66:
			ret = this.Syscalls.maReadStore(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 67:
			ret = this.Syscalls.maCloseStore(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 68:
			ret = this.Syscalls.maConnect(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 69:
			ret = this.Syscalls.maConnClose(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 70:
			ret = this.Syscalls.maConnRead(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 71:
			ret = this.Syscalls.maConnWrite(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 72:
			ret = this.Syscalls.maConnReadToData(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 73:
			ret = this.Syscalls.maConnWriteFromData(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 74:
			ret = this.Syscalls.maConnGetAddr(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 75:
			ret = this.Syscalls.maHttpCreate(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 76:
			ret = this.Syscalls.maHttpSetRequestHeader(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 77:
			ret = this.Syscalls.maHttpGetResponseHeader(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 78:
			ret = this.Syscalls.maHttpFinish(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 79:
			ret = this.Syscalls.maLoadResources(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 80:
			ret = this.Syscalls.maLoadProgram(this.regs[Reg.i0], this.regs[Reg.i1]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 81:
			ret = this.Syscalls.maGetKeys();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 82:
			ret = this.Syscalls.maGetEvent(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 83:
			ret = this.Syscalls.maWait(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 84:
			ret = this.Syscalls.maTime();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 85:
			ret = this.Syscalls.maLocalTime();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 86:
			ret = this.Syscalls.maGetMilliSecondCount();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 87:
			ret = this.Syscalls.maFreeObjectMemory();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 88:
			ret = this.Syscalls.maTotalObjectMemory();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 89:
			ret = this.Syscalls.maVibrate(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 90:
			ret = this.Syscalls.maSoundPlay(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 91:
			ret = this.Syscalls.maSoundStop();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 92:
			ret = this.Syscalls.maSoundIsPlaying();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 93:
			ret = this.Syscalls.maSoundGetVolume();
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 94:
			ret = this.Syscalls.maSoundSetVolume(this.regs[Reg.i0]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 95:
			ret = this.Syscalls.maInvokeExtension(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
		case 96:
			ret = this.Syscalls.maIOCtl(this.regs[Reg.i0], this.regs[Reg.i1], this.regs[Reg.i2], this.regs[Reg.i3]);
			this.regs[Reg.r14] = (ret&0xffffffff);
		break;
	};
};
MoSyncGenerated.maIOCtl = function(id, i1, i2, i3) {
	switch(id) {
		case 1:
			if(this.maFontLoadDefault == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFontLoadDefault(i1, i2, i3);
		case 2:
			if(this.maFontSetCurrent == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFontSetCurrent(i1);
		case 3:
			if(this.maFontGetCount == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFontGetCount();
		case 4:
			if(this.maFontGetName == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFontGetName(i1, i2, i3);
		case 5:
			if(this.maFontLoadWithName == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFontLoadWithName(i1, i2);
		case 6:
			if(this.maFontDelete == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFontDelete(i1);
		case 7:
			if(this.maReportCallStack == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maReportCallStack();
		case 8:
			if(this.maDumpCallStackEx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maDumpCallStackEx(i1, i2);
		case 9:
			if(this.maProtectMemory == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maProtectMemory(i1, i2);
		case 10:
			if(this.maUnprotectMemory == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maUnprotectMemory(i1, i2);
		case 11:
			if(this.maSetMemoryProtection == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSetMemoryProtection(i1);
		case 12:
			if(this.maGetMemoryProtection == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maGetMemoryProtection();
		case 13:
			if(this.maGetBatteryCharge == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maGetBatteryCharge();
		case 14:
			if(this.maStartSpeaking == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStartSpeaking(i1);
		case 15:
			if(this.maStopSpeaking == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStopSpeaking();
		case 16:
			if(this.maIsSpeaking == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maIsSpeaking();
		case 17:
			if(this.maSetSpeechVolume == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSetSpeechVolume(i1);
		case 18:
			if(this.maSetSpeechSpeed == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSetSpeechSpeed(i1);
		case 19:
			if(this.maSetSpeechPitch == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSetSpeechPitch(i1);
		case 20:
			if(this.sinh == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.sinh(i1, i2);
		case 21:
			if(this.cosh == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.cosh(i1, i2);
		case 22:
			if(this.atanh == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.atanh(i1, i2);
		case 23:
			if(this.maLockKeypad == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maLockKeypad();
		case 24:
			if(this.maUnlockKeypad == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maUnlockKeypad();
		case 25:
			if(this.maKeypadIsLocked == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maKeypadIsLocked();
		case 26:
			if(this.maWriteLog == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWriteLog(i1, i2);
		case 27:
			if(this.maBtStartDeviceDiscovery == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maBtStartDeviceDiscovery(i1);
		case 28:
			if(this.maBtGetNewDevice == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maBtGetNewDevice(i1);
		case 29:
			if(this.maBtStartServiceDiscovery == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maBtStartServiceDiscovery(i1, i2);
		case 30:
			if(this.maBtGetNextServiceSize == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maBtGetNextServiceSize(i1);
		case 31:
			if(this.maBtGetNewService == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maBtGetNewService(i1);
		case 32:
			if(this.maBtCancelDiscovery == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maBtCancelDiscovery();
		case 33:
			if(this.maWlanStartDiscovery == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWlanStartDiscovery();
		case 34:
			if(this.maWlanGetNewAp == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWlanGetNewAp(i1);
		case 35:
			if(this.maLocationStart == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maLocationStart();
		case 36:
			if(this.maLocationStop == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maLocationStop();
		case 37:
			if(this.maPlatformRequest == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPlatformRequest(i1);
		case 38:
			if(this.maFileOpen == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileOpen(i1, i2);
		case 39:
			if(this.maFileExists == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileExists(i1);
		case 40:
			if(this.maFileClose == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileClose(i1);
		case 41:
			if(this.maFileCreate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileCreate(i1);
		case 42:
			if(this.maFileDelete == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileDelete(i1);
		case 43:
			if(this.maFileSize == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileSize(i1);
		case 44:
			if(this.maFileAvailableSpace == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileAvailableSpace(i1);
		case 45:
			if(this.maFileTotalSpace == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileTotalSpace(i1);
		case 46:
			if(this.maFileDate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileDate(i1);
		case 47:
			if(this.maFileRename == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileRename(i1, i2);
		case 48:
			if(this.maFileTruncate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileTruncate(i1, i2);
		case 49:
			if(this.maFileWrite == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileWrite(i1, i2, i3);
		case 50:
			if(this.maFileWriteFromData == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileWriteFromData(i1, i2, i3, this.getStackValue(0));
		case 51:
			if(this.maFileRead == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileRead(i1, i2, i3);
		case 52:
			if(this.maFileReadToData == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileReadToData(i1, i2, i3, this.getStackValue(0));
		case 53:
			if(this.maFileTell == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileTell(i1);
		case 54:
			if(this.maFileSeek == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileSeek(i1, i2, i3);
		case 55:
			if(this.maFileListStart == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileListStart(i1, i2, i3);
		case 56:
			if(this.maFileListNext == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileListNext(i1, i2, i3);
		case 57:
			if(this.maFileListClose == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFileListClose(i1);
		case 58:
			if(this.maSendTextSMS == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSendTextSMS(i1, i2);
		case 59:
			if(this.maRecordSetup == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maRecordSetup();
		case 60:
			if(this.maRecordStart == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maRecordStart(i1);
		case 61:
			if(this.maRecordPause == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maRecordPause(i1);
		case 62:
			if(this.maGetCellInfo == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maGetCellInfo(i1);
		case 63:
			if(this.maCallDial == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCallDial(i1);
		case 64:
			if(this.maCallAnswer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCallAnswer();
		case 65:
			if(this.maCallHangup == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCallHangup();
		case 66:
			if(this.maSoundPlayConn == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSoundPlayConn(i1, i2);
		case 67:
			if(this.maStreamVideoStart == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStreamVideoStart(i1);
		case 68:
			if(this.maStreamVideoSize == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStreamVideoSize(i1);
		case 69:
			if(this.maStreamVideoSetFrame == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStreamVideoSetFrame(i1, i2);
		case 70:
			if(this.maStreamLength == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStreamLength(i1);
		case 71:
			if(this.maStreamPos == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStreamPos(i1);
		case 72:
			if(this.maStreamSetPos == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStreamSetPos(i1, i2);
		case 73:
			if(this.maStreamPause == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStreamPause(i1);
		case 74:
			if(this.maStreamResume == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStreamResume(i1);
		case 75:
			if(this.maStreamClose == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maStreamClose(i1);
		case 76:
			if(this.maFrameBufferGetInfo == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFrameBufferGetInfo(i1);
		case 77:
			if(this.maFrameBufferInit == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFrameBufferInit(i1);
		case 78:
			if(this.maFrameBufferClose == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maFrameBufferClose();
		case 79:
			if(this.maAudioBufferInit == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maAudioBufferInit(i1);
		case 80:
			if(this.maAudioBufferReady == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maAudioBufferReady();
		case 81:
			if(this.maAudioBufferClose == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maAudioBufferClose();
		case 82:
			if(this.maAccept == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maAccept(i1);
		case 83:
			if(this.maSendToBackground == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSendToBackground();
		case 84:
			if(this.maBringToForeground == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maBringToForeground();
		case 85:
			if(this.maAddDataSize == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maAddDataSize(i1, i2);
		case 86:
			if(this.maGetSystemProperty == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maGetSystemProperty(i1, i2, i3);
		case 87:
			if(this.maPimListOpen == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimListOpen(i1);
		case 88:
			if(this.maPimListNext == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimListNext(i1);
		case 89:
			if(this.maPimListClose == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimListClose(i1);
		case 90:
			if(this.maPimItemCount == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemCount(i1);
		case 91:
			if(this.maPimItemGetField == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemGetField(i1, i2);
		case 92:
			if(this.maPimItemFieldCount == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemFieldCount(i1, i2);
		case 93:
			if(this.maPimItemGetAttributes == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemGetAttributes(i1, i2, i3);
		case 94:
			if(this.maPimItemSetLabel == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemSetLabel(i1, i2);
		case 95:
			if(this.maPimItemGetLabel == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemGetLabel(i1, i2);
		case 96:
			if(this.maPimFieldType == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimFieldType(i1, i2);
		case 97:
			if(this.maPimItemGetValue == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemGetValue(i1, i2);
		case 98:
			if(this.maPimItemSetValue == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemSetValue(i1, i2, i3);
		case 99:
			if(this.maPimItemAddValue == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemAddValue(i1, i2);
		case 100:
			if(this.maPimItemRemoveValue == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemRemoveValue(i1, i2, i3);
		case 101:
			if(this.maPimItemClose == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemClose(i1);
		case 102:
			if(this.maPimItemCreate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemCreate(i1);
		case 103:
			if(this.maPimItemRemove == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maPimItemRemove(i1, i2);
		case 104:
			if(this.maWidgetCreate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetCreate(i1);
		case 105:
			if(this.maWidgetDestroy == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetDestroy(i1);
		case 106:
			if(this.maWidgetAddChild == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetAddChild(i1, i2);
		case 107:
			if(this.maWidgetInsertChild == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetInsertChild(i1, i2, i3);
		case 108:
			if(this.maWidgetRemoveChild == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetRemoveChild(i1);
		case 109:
			if(this.maWidgetModalDialogShow == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetModalDialogShow(i1);
		case 110:
			if(this.maWidgetModalDialogHide == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetModalDialogHide(i1);
		case 111:
			if(this.maWidgetScreenShow == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetScreenShow(i1);
		case 112:
			if(this.maWidgetStackScreenPush == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetStackScreenPush(i1, i2);
		case 113:
			if(this.maWidgetStackScreenPop == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetStackScreenPop(i1);
		case 114:
			if(this.maWidgetSetProperty == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetSetProperty(i1, i2, i3);
		case 115:
			if(this.maWidgetGetProperty == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maWidgetGetProperty(i1, i2, i3, this.getStackValue(0));
		case 116:
			if(this.maCameraFormatNumber == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraFormatNumber();
		case 117:
			if(this.maCameraFormat == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraFormat(i1, i2);
		case 118:
			if(this.maCameraStart == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraStart();
		case 119:
			if(this.maCameraStop == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraStop();
		case 120:
			if(this.maCameraSetPreview == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraSetPreview(i1);
		case 121:
			if(this.maCameraSelect == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraSelect(i1);
		case 122:
			if(this.maCameraNumber == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraNumber();
		case 123:
			if(this.maCameraSnapshot == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraSnapshot(i1, i2);
		case 124:
			if(this.maCameraRecord == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraRecord(i1);
		case 125:
			if(this.maCameraSetProperty == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraSetProperty(i1, i2);
		case 126:
			if(this.maCameraGetProperty == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maCameraGetProperty(i1, i2, i3);
		case 127:
			if(this.maShowVirtualKeyboard == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maShowVirtualKeyboard();
		case 128:
			if(this.maTextBox == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maTextBox(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 129:
			if(this.maKeyCaptureStart == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maKeyCaptureStart(i1);
		case 130:
			if(this.maKeyCaptureStop == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maKeyCaptureStop();
		case 131:
			if(this.maHomeScreenEventsOn == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maHomeScreenEventsOn();
		case 132:
			if(this.maHomeScreenEventsOff == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maHomeScreenEventsOff();
		case 133:
			if(this.maHomeScreenShortcutAdd == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maHomeScreenShortcutAdd(i1);
		case 134:
			if(this.maHomeScreenShortcutRemove == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maHomeScreenShortcutRemove(i1);
		case 135:
			if(this.maNotificationAdd == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNotificationAdd(i1, i2, i3, this.getStackValue(0));
		case 136:
			if(this.maNotificationRemove == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNotificationRemove(i1);
		case 137:
			if(this.maScreenSetOrientation == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maScreenSetOrientation(i1);
		case 138:
			if(this.maScreenSetFullscreen == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maScreenSetFullscreen(i1);
		case 139:
			if(this.maScreenStateEventsOn == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maScreenStateEventsOn();
		case 140:
			if(this.maScreenStateEventsOff == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maScreenStateEventsOff();
		case 141:
			if(this.maOpenGLInitFullscreen == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maOpenGLInitFullscreen(i1);
		case 142:
			if(this.maOpenGLCloseFullscreen == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maOpenGLCloseFullscreen();
		case 143:
			if(this.maOpenGLTexImage2D == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maOpenGLTexImage2D(i1);
		case 144:
			if(this.maOpenGLTexSubImage2D == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maOpenGLTexSubImage2D(i1);
		case 145:
			if(this.glActiveTexture == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glActiveTexture(i1);
		case 146:
			if(this.glBindBuffer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBindBuffer(i1, i2);
		case 147:
			if(this.glBindTexture == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBindTexture(i1, i2);
		case 148:
			if(this.glBlendFunc == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBlendFunc(i1, i2);
		case 149:
			if(this.glBufferData == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBufferData(i1, i2, i3, this.getStackValue(0));
		case 150:
			if(this.glBufferSubData == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBufferSubData(i1, i2, i3, this.getStackValue(0));
		case 151:
			if(this.glClear == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glClear(i1);
		case 152:
			if(this.glClearColor == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glClearColor(i1, i2, i3, this.getStackValue(0));
		case 153:
			if(this.glClearDepthf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glClearDepthf(i1);
		case 154:
			if(this.glClearStencil == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glClearStencil(i1);
		case 155:
			if(this.glColorMask == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glColorMask(i1, i2, i3, this.getStackValue(0));
		case 156:
			if(this.glCompressedTexImage2D == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCompressedTexImage2D(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8), this.getStackValue(12), this.getStackValue(16));
		case 157:
			if(this.glCompressedTexSubImage2D == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCompressedTexSubImage2D(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8), this.getStackValue(12), this.getStackValue(16), this.getStackValue(20));
		case 158:
			if(this.glCopyTexImage2D == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCopyTexImage2D(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8), this.getStackValue(12), this.getStackValue(16));
		case 159:
			if(this.glCopyTexSubImage2D == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCopyTexSubImage2D(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8), this.getStackValue(12), this.getStackValue(16));
		case 160:
			if(this.glCullFace == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCullFace(i1);
		case 161:
			if(this.glDeleteBuffers == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDeleteBuffers(i1, i2);
		case 162:
			if(this.glDeleteTextures == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDeleteTextures(i1, i2);
		case 163:
			if(this.glDepthFunc == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDepthFunc(i1);
		case 164:
			if(this.glDepthMask == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDepthMask(i1);
		case 165:
			if(this.glDepthRangef == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDepthRangef(i1, i2);
		case 166:
			if(this.glDisable == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDisable(i1);
		case 167:
			if(this.glDrawArrays == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDrawArrays(i1, i2, i3);
		case 168:
			if(this.glDrawElements == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDrawElements(i1, i2, i3, this.getStackValue(0));
		case 169:
			if(this.glEnable == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glEnable(i1);
		case 170:
			if(this.glFinish == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFinish();
		case 171:
			if(this.glFlush == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFlush();
		case 172:
			if(this.glFrontFace == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFrontFace(i1);
		case 173:
			if(this.glGenBuffers == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGenBuffers(i1, i2);
		case 174:
			if(this.glGenTextures == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGenTextures(i1, i2);
		case 175:
			if(this.glGetBooleanv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetBooleanv(i1, i2);
		case 176:
			if(this.glGetBufferParameteriv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetBufferParameteriv(i1, i2, i3);
		case 177:
			if(this.glGetError == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetError();
		case 178:
			if(this.glGetFloatv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetFloatv(i1, i2);
		case 179:
			if(this.glGetIntegerv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetIntegerv(i1, i2);
		case 180:
			if(this.glGetStringHandle == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetStringHandle(i1);
		case 181:
			if(this.glGetTexParameterfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetTexParameterfv(i1, i2, i3);
		case 182:
			if(this.glGetTexParameteriv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetTexParameteriv(i1, i2, i3);
		case 183:
			if(this.glHint == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glHint(i1, i2);
		case 184:
			if(this.glIsBuffer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glIsBuffer(i1);
		case 185:
			if(this.glIsEnabled == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glIsEnabled(i1);
		case 186:
			if(this.glIsTexture == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glIsTexture(i1);
		case 187:
			if(this.glLineWidth == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLineWidth(i1);
		case 188:
			if(this.glPixelStorei == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPixelStorei(i1, i2);
		case 189:
			if(this.glPolygonOffset == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPolygonOffset(i1, i2);
		case 190:
			if(this.glReadPixels == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glReadPixels(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8), this.getStackValue(12));
		case 191:
			if(this.glSampleCoverage == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glSampleCoverage(i1, i2);
		case 192:
			if(this.glScissor == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glScissor(i1, i2, i3, this.getStackValue(0));
		case 193:
			if(this.glStencilFunc == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glStencilFunc(i1, i2, i3);
		case 194:
			if(this.glStencilMask == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glStencilMask(i1);
		case 195:
			if(this.glStencilOp == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glStencilOp(i1, i2, i3);
		case 196:
			if(this.glTexImage2D == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexImage2D(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8), this.getStackValue(12), this.getStackValue(16), this.getStackValue(20));
		case 197:
			if(this.glTexParameterf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexParameterf(i1, i2, i3);
		case 198:
			if(this.glTexParameterfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexParameterfv(i1, i2, i3);
		case 199:
			if(this.glTexParameteri == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexParameteri(i1, i2, i3);
		case 200:
			if(this.glTexParameteriv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexParameteriv(i1, i2, i3);
		case 201:
			if(this.glTexSubImage2D == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexSubImage2D(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8), this.getStackValue(12), this.getStackValue(16), this.getStackValue(20));
		case 202:
			if(this.glViewport == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glViewport(i1, i2, i3, this.getStackValue(0));
		case 203:
			if(this.glAttachShader == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glAttachShader(i1, i2);
		case 204:
			if(this.glBindAttribLocation == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBindAttribLocation(i1, i2, i3);
		case 205:
			if(this.glBindFramebuffer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBindFramebuffer(i1, i2);
		case 206:
			if(this.glBindRenderbuffer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBindRenderbuffer(i1, i2);
		case 207:
			if(this.glBlendColor == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBlendColor(i1, i2, i3, this.getStackValue(0));
		case 208:
			if(this.glBlendEquation == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBlendEquation(i1);
		case 209:
			if(this.glBlendEquationSeparate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBlendEquationSeparate(i1, i2);
		case 210:
			if(this.glBlendFuncSeparate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBlendFuncSeparate(i1, i2, i3, this.getStackValue(0));
		case 211:
			if(this.glCheckFramebufferStatus == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCheckFramebufferStatus(i1);
		case 212:
			if(this.glCompileShader == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCompileShader(i1);
		case 213:
			if(this.glCreateProgram == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCreateProgram();
		case 214:
			if(this.glCreateShader == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCreateShader(i1);
		case 215:
			if(this.glDeleteFramebuffers == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDeleteFramebuffers(i1, i2);
		case 216:
			if(this.glDeleteProgram == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDeleteProgram(i1);
		case 217:
			if(this.glDeleteRenderbuffers == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDeleteRenderbuffers(i1, i2);
		case 218:
			if(this.glDeleteShader == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDeleteShader(i1);
		case 219:
			if(this.glDetachShader == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDetachShader(i1, i2);
		case 220:
			if(this.glDisableVertexAttribArray == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDisableVertexAttribArray(i1);
		case 221:
			if(this.glEnableVertexAttribArray == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glEnableVertexAttribArray(i1);
		case 222:
			if(this.glFramebufferRenderbuffer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFramebufferRenderbuffer(i1, i2, i3, this.getStackValue(0));
		case 223:
			if(this.glFramebufferTexture2D == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFramebufferTexture2D(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 224:
			if(this.glGenerateMipmap == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGenerateMipmap(i1);
		case 225:
			if(this.glGenFramebuffers == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGenFramebuffers(i1, i2);
		case 226:
			if(this.glGenRenderbuffers == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGenRenderbuffers(i1, i2);
		case 227:
			if(this.glGetActiveAttrib == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetActiveAttrib(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8), this.getStackValue(12));
		case 228:
			if(this.glGetActiveUniform == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetActiveUniform(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8), this.getStackValue(12));
		case 229:
			if(this.glGetAttachedShaders == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetAttachedShaders(i1, i2, i3, this.getStackValue(0));
		case 230:
			if(this.glGetAttribLocation == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetAttribLocation(i1, i2);
		case 231:
			if(this.glGetFramebufferAttachmentParameteriv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetFramebufferAttachmentParameteriv(i1, i2, i3, this.getStackValue(0));
		case 232:
			if(this.glGetProgramiv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetProgramiv(i1, i2, i3);
		case 233:
			if(this.glGetProgramInfoLog == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetProgramInfoLog(i1, i2, i3, this.getStackValue(0));
		case 234:
			if(this.glGetRenderbufferParameteriv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetRenderbufferParameteriv(i1, i2, i3);
		case 235:
			if(this.glGetShaderiv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetShaderiv(i1, i2, i3);
		case 236:
			if(this.glGetShaderInfoLog == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetShaderInfoLog(i1, i2, i3, this.getStackValue(0));
		case 237:
			if(this.glGetShaderPrecisionFormat == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetShaderPrecisionFormat(i1, i2, i3, this.getStackValue(0));
		case 238:
			if(this.glGetShaderSource == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetShaderSource(i1, i2, i3, this.getStackValue(0));
		case 239:
			if(this.glGetUniformfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetUniformfv(i1, i2, i3);
		case 240:
			if(this.glGetUniformiv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetUniformiv(i1, i2, i3);
		case 241:
			if(this.glGetUniformLocation == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetUniformLocation(i1, i2);
		case 242:
			if(this.glGetVertexAttribfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetVertexAttribfv(i1, i2, i3);
		case 243:
			if(this.glGetVertexAttribiv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetVertexAttribiv(i1, i2, i3);
		case 244:
			if(this.glGetVertexAttribPointerv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetVertexAttribPointerv(i1, i2, i3);
		case 245:
			if(this.glIsFramebuffer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glIsFramebuffer(i1);
		case 246:
			if(this.glIsProgram == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glIsProgram(i1);
		case 247:
			if(this.glIsRenderbuffer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glIsRenderbuffer(i1);
		case 248:
			if(this.glIsShader == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glIsShader(i1);
		case 249:
			if(this.glLinkProgram == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLinkProgram(i1);
		case 250:
			if(this.glReleaseShaderCompiler == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glReleaseShaderCompiler();
		case 251:
			if(this.glRenderbufferStorage == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glRenderbufferStorage(i1, i2, i3, this.getStackValue(0));
		case 252:
			if(this.glShaderBinary == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glShaderBinary(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 253:
			if(this.glShaderSource == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glShaderSource(i1, i2, i3, this.getStackValue(0));
		case 254:
			if(this.glStencilFuncSeparate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glStencilFuncSeparate(i1, i2, i3, this.getStackValue(0));
		case 255:
			if(this.glStencilMaskSeparate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glStencilMaskSeparate(i1, i2);
		case 256:
			if(this.glStencilOpSeparate == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glStencilOpSeparate(i1, i2, i3, this.getStackValue(0));
		case 257:
			if(this.glUniform1f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform1f(i1, i2);
		case 258:
			if(this.glUniform1fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform1fv(i1, i2, i3);
		case 259:
			if(this.glUniform1i == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform1i(i1, i2);
		case 260:
			if(this.glUniform1iv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform1iv(i1, i2, i3);
		case 261:
			if(this.glUniform2f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform2f(i1, i2, i3);
		case 262:
			if(this.glUniform2fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform2fv(i1, i2, i3);
		case 263:
			if(this.glUniform2i == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform2i(i1, i2, i3);
		case 264:
			if(this.glUniform2iv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform2iv(i1, i2, i3);
		case 265:
			if(this.glUniform3f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform3f(i1, i2, i3, this.getStackValue(0));
		case 266:
			if(this.glUniform3fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform3fv(i1, i2, i3);
		case 267:
			if(this.glUniform3i == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform3i(i1, i2, i3, this.getStackValue(0));
		case 268:
			if(this.glUniform3iv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform3iv(i1, i2, i3);
		case 269:
			if(this.glUniform4f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform4f(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 270:
			if(this.glUniform4fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform4fv(i1, i2, i3);
		case 271:
			if(this.glUniform4i == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform4i(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 272:
			if(this.glUniform4iv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniform4iv(i1, i2, i3);
		case 273:
			if(this.glUniformMatrix2fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniformMatrix2fv(i1, i2, i3, this.getStackValue(0));
		case 274:
			if(this.glUniformMatrix3fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniformMatrix3fv(i1, i2, i3, this.getStackValue(0));
		case 275:
			if(this.glUniformMatrix4fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUniformMatrix4fv(i1, i2, i3, this.getStackValue(0));
		case 276:
			if(this.glUseProgram == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glUseProgram(i1);
		case 277:
			if(this.glValidateProgram == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glValidateProgram(i1);
		case 278:
			if(this.glVertexAttrib1f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexAttrib1f(i1, i2);
		case 279:
			if(this.glVertexAttrib1fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexAttrib1fv(i1, i2);
		case 280:
			if(this.glVertexAttrib2f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexAttrib2f(i1, i2, i3);
		case 281:
			if(this.glVertexAttrib2fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexAttrib2fv(i1, i2);
		case 282:
			if(this.glVertexAttrib3f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexAttrib3f(i1, i2, i3, this.getStackValue(0));
		case 283:
			if(this.glVertexAttrib3fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexAttrib3fv(i1, i2);
		case 284:
			if(this.glVertexAttrib4f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexAttrib4f(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 285:
			if(this.glVertexAttrib4fv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexAttrib4fv(i1, i2);
		case 286:
			if(this.glVertexAttribPointer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexAttribPointer(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8));
		case 287:
			if(this.glAlphaFunc == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glAlphaFunc(i1, i2);
		case 288:
			if(this.glClipPlanef == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glClipPlanef(i1, i2);
		case 289:
			if(this.glColor4f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glColor4f(i1, i2, i3, this.getStackValue(0));
		case 290:
			if(this.glFogf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFogf(i1, i2);
		case 291:
			if(this.glFogfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFogfv(i1, i2);
		case 292:
			if(this.glFrustumf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFrustumf(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8));
		case 293:
			if(this.glGetClipPlanef == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetClipPlanef(i1, i2);
		case 294:
			if(this.glGetLightfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetLightfv(i1, i2, i3);
		case 295:
			if(this.glGetMaterialfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetMaterialfv(i1, i2, i3);
		case 296:
			if(this.glGetTexEnvfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetTexEnvfv(i1, i2, i3);
		case 297:
			if(this.glLightModelf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLightModelf(i1, i2);
		case 298:
			if(this.glLightModelfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLightModelfv(i1, i2);
		case 299:
			if(this.glLightf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLightf(i1, i2, i3);
		case 300:
			if(this.glLightfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLightfv(i1, i2, i3);
		case 301:
			if(this.glLoadMatrixf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLoadMatrixf(i1);
		case 302:
			if(this.glMaterialf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glMaterialf(i1, i2, i3);
		case 303:
			if(this.glMaterialfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glMaterialfv(i1, i2, i3);
		case 304:
			if(this.glMultMatrixf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glMultMatrixf(i1);
		case 305:
			if(this.glMultiTexCoord4f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glMultiTexCoord4f(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 306:
			if(this.glNormal3f == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glNormal3f(i1, i2, i3);
		case 307:
			if(this.glOrthof == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glOrthof(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8));
		case 308:
			if(this.glPointParameterf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPointParameterf(i1, i2);
		case 309:
			if(this.glPointParameterfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPointParameterfv(i1, i2);
		case 310:
			if(this.glPointSize == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPointSize(i1);
		case 311:
			if(this.glRotatef == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glRotatef(i1, i2, i3, this.getStackValue(0));
		case 312:
			if(this.glScalef == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glScalef(i1, i2, i3);
		case 313:
			if(this.glTexEnvf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexEnvf(i1, i2, i3);
		case 314:
			if(this.glTexEnvfv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexEnvfv(i1, i2, i3);
		case 315:
			if(this.glTranslatef == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTranslatef(i1, i2, i3);
		case 316:
			if(this.glAlphaFuncx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glAlphaFuncx(i1, i2);
		case 317:
			if(this.glClearColorx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glClearColorx(i1, i2, i3, this.getStackValue(0));
		case 318:
			if(this.glClearDepthx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glClearDepthx(i1);
		case 319:
			if(this.glClientActiveTexture == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glClientActiveTexture(i1);
		case 320:
			if(this.glClipPlanex == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glClipPlanex(i1, i2);
		case 321:
			if(this.glColor4ub == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glColor4ub(i1, i2, i3, this.getStackValue(0));
		case 322:
			if(this.glColor4x == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glColor4x(i1, i2, i3, this.getStackValue(0));
		case 323:
			if(this.glColorPointer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glColorPointer(i1, i2, i3, this.getStackValue(0));
		case 324:
			if(this.glDepthRangex == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDepthRangex(i1, i2);
		case 325:
			if(this.glDisableClientState == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDisableClientState(i1);
		case 326:
			if(this.glEnableClientState == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glEnableClientState(i1);
		case 327:
			if(this.glFogx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFogx(i1, i2);
		case 328:
			if(this.glFogxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFogxv(i1, i2);
		case 329:
			if(this.glFrustumx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFrustumx(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8));
		case 330:
			if(this.glGetClipPlanex == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetClipPlanex(i1, i2);
		case 331:
			if(this.glGetFixedv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetFixedv(i1, i2);
		case 332:
			if(this.glGetLightxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetLightxv(i1, i2, i3);
		case 333:
			if(this.glGetMaterialxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetMaterialxv(i1, i2, i3);
		case 334:
			if(this.glGetPointerv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetPointerv(i1, i2);
		case 335:
			if(this.glGetTexEnviv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetTexEnviv(i1, i2, i3);
		case 336:
			if(this.glGetTexEnvxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetTexEnvxv(i1, i2, i3);
		case 337:
			if(this.glGetTexParameterxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetTexParameterxv(i1, i2, i3);
		case 338:
			if(this.glLightModelx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLightModelx(i1, i2);
		case 339:
			if(this.glLightModelxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLightModelxv(i1, i2);
		case 340:
			if(this.glLightx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLightx(i1, i2, i3);
		case 341:
			if(this.glLightxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLightxv(i1, i2, i3);
		case 342:
			if(this.glLineWidthx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLineWidthx(i1);
		case 343:
			if(this.glLoadIdentity == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLoadIdentity();
		case 344:
			if(this.glLoadMatrixx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLoadMatrixx(i1);
		case 345:
			if(this.glLogicOp == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glLogicOp(i1);
		case 346:
			if(this.glMaterialx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glMaterialx(i1, i2, i3);
		case 347:
			if(this.glMaterialxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glMaterialxv(i1, i2, i3);
		case 348:
			if(this.glMatrixMode == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glMatrixMode(i1);
		case 349:
			if(this.glMultMatrixx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glMultMatrixx(i1);
		case 350:
			if(this.glMultiTexCoord4x == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glMultiTexCoord4x(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 351:
			if(this.glNormal3x == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glNormal3x(i1, i2, i3);
		case 352:
			if(this.glNormalPointer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glNormalPointer(i1, i2, i3);
		case 353:
			if(this.glOrthox == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glOrthox(i1, i2, i3, this.getStackValue(0), this.getStackValue(4), this.getStackValue(8));
		case 354:
			if(this.glPointParameterx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPointParameterx(i1, i2);
		case 355:
			if(this.glPointParameterxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPointParameterxv(i1, i2);
		case 356:
			if(this.glPointSizex == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPointSizex(i1);
		case 357:
			if(this.glPolygonOffsetx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPolygonOffsetx(i1, i2);
		case 358:
			if(this.glPopMatrix == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPopMatrix();
		case 359:
			if(this.glPushMatrix == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPushMatrix();
		case 360:
			if(this.glRotatex == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glRotatex(i1, i2, i3, this.getStackValue(0));
		case 361:
			if(this.glSampleCoveragex == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glSampleCoveragex(i1, i2);
		case 362:
			if(this.glScalex == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glScalex(i1, i2, i3);
		case 363:
			if(this.glShadeModel == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glShadeModel(i1);
		case 364:
			if(this.glTexCoordPointer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexCoordPointer(i1, i2, i3, this.getStackValue(0));
		case 365:
			if(this.glTexEnvi == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexEnvi(i1, i2, i3);
		case 366:
			if(this.glTexEnvx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexEnvx(i1, i2, i3);
		case 367:
			if(this.glTexEnviv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexEnviv(i1, i2, i3);
		case 368:
			if(this.glTexEnvxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexEnvxv(i1, i2, i3);
		case 369:
			if(this.glTexParameterx == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexParameterx(i1, i2, i3);
		case 370:
			if(this.glTexParameterxv == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTexParameterxv(i1, i2, i3);
		case 371:
			if(this.glTranslatex == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glTranslatex(i1, i2, i3);
		case 372:
			if(this.glVertexPointer == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glVertexPointer(i1, i2, i3, this.getStackValue(0));
		case 373:
			if(this.glPointSizePointerOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glPointSizePointerOES(i1, i2, i3);
		case 374:
			if(this.glIsRenderbufferOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glIsRenderbufferOES(i1);
		case 375:
			if(this.glBindRenderbufferOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBindRenderbufferOES(i1, i2);
		case 376:
			if(this.glDeleteRenderbuffersOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDeleteRenderbuffersOES(i1, i2);
		case 377:
			if(this.glGenRenderbuffersOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGenRenderbuffersOES(i1, i2);
		case 378:
			if(this.glRenderbufferStorageOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glRenderbufferStorageOES(i1, i2, i3, this.getStackValue(0));
		case 379:
			if(this.glGetRenderbufferParameterivOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetRenderbufferParameterivOES(i1, i2, i3);
		case 380:
			if(this.glIsFramebufferOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glIsFramebufferOES(i1);
		case 381:
			if(this.glBindFramebufferOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glBindFramebufferOES(i1, i2);
		case 382:
			if(this.glDeleteFramebuffersOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glDeleteFramebuffersOES(i1, i2);
		case 383:
			if(this.glGenFramebuffersOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGenFramebuffersOES(i1, i2);
		case 384:
			if(this.glCheckFramebufferStatusOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glCheckFramebufferStatusOES(i1);
		case 385:
			if(this.glFramebufferRenderbufferOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFramebufferRenderbufferOES(i1, i2, i3, this.getStackValue(0));
		case 386:
			if(this.glFramebufferTexture2DOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glFramebufferTexture2DOES(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 387:
			if(this.glGetFramebufferAttachmentParameterivOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGetFramebufferAttachmentParameterivOES(i1, i2, i3, this.getStackValue(0));
		case 388:
			if(this.glGenerateMipmapOES == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.glGenerateMipmapOES(i1);
		case 389:
			if(this.maReportResourceInformation == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maReportResourceInformation();
		case 390:
			if(this.maMessageBox == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maMessageBox(i1, i2);
		case 391:
			if(this.maAlert == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maAlert(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 392:
			if(this.maImagePickerOpen == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maImagePickerOpen();
		case 393:
			if(this.maOptionsBox == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maOptionsBox(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 394:
			if(this.maSensorStart == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSensorStart(i1, i2);
		case 395:
			if(this.maSensorStop == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSensorStop(i1);
		case 396:
			if(this.maNFCStart == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCStart();
		case 397:
			if(this.maNFCStop == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCStop();
		case 398:
			if(this.maNFCReadTag == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCReadTag(i1);
		case 399:
			if(this.maNFCDestroyTag == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCDestroyTag(i1);
		case 400:
			if(this.maNFCConnectTag == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCConnectTag(i1);
		case 401:
			if(this.maNFCCloseTag == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCCloseTag(i1);
		case 402:
			if(this.maNFCIsType == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCIsType(i1, i2);
		case 403:
			if(this.maNFCGetTypedTag == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetTypedTag(i1, i2);
		case 404:
			if(this.maNFCBatchStart == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCBatchStart(i1);
		case 405:
			if(this.maNFCBatchCommit == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCBatchCommit(i1);
		case 406:
			if(this.maNFCBatchRollback == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCBatchRollback(i1);
		case 407:
			if(this.maNFCTransceive == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCTransceive(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 408:
			if(this.maNFCSetReadOnly == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCSetReadOnly(i1);
		case 409:
			if(this.maNFCIsReadOnly == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCIsReadOnly(i1);
		case 410:
			if(this.maNFCGetSize == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetSize(i1);
		case 411:
			if(this.maNFCReadNDEFMessage == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCReadNDEFMessage(i1);
		case 412:
			if(this.maNFCWriteNDEFMessage == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCWriteNDEFMessage(i1, i2);
		case 413:
			if(this.maNFCCreateNDEFMessage == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCCreateNDEFMessage(i1);
		case 414:
			if(this.maNFCGetNDEFMessage == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetNDEFMessage(i1);
		case 415:
			if(this.maNFCGetNDEFRecord == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetNDEFRecord(i1, i2);
		case 416:
			if(this.maNFCGetNDEFRecordCount == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetNDEFRecordCount(i1);
		case 417:
			if(this.maNFCGetNDEFId == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetNDEFId(i1, i2, i3);
		case 418:
			if(this.maNFCGetNDEFPayload == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetNDEFPayload(i1, i2, i3);
		case 419:
			if(this.maNFCGetNDEFTnf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetNDEFTnf(i1);
		case 420:
			if(this.maNFCGetNDEFType == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetNDEFType(i1, i2, i3);
		case 421:
			if(this.maNFCSetNDEFId == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCSetNDEFId(i1, i2, i3);
		case 422:
			if(this.maNFCSetNDEFPayload == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCSetNDEFPayload(i1, i2, i3);
		case 423:
			if(this.maNFCSetNDEFTnf == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCSetNDEFTnf(i1, i2);
		case 424:
			if(this.maNFCSetNDEFType == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCSetNDEFType(i1, i2, i3);
		case 425:
			if(this.maNFCAuthenticateMifareSector == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCAuthenticateMifareSector(i1, i2, i3, this.getStackValue(0), this.getStackValue(4));
		case 426:
			if(this.maNFCGetMifareSectorCount == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetMifareSectorCount(i1);
		case 427:
			if(this.maNFCGetMifareBlockCountInSector == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCGetMifareBlockCountInSector(i1, i2);
		case 428:
			if(this.maNFCMifareSectorToBlock == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCMifareSectorToBlock(i1, i2);
		case 429:
			if(this.maNFCReadMifareBlocks == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCReadMifareBlocks(i1, i2, i3, this.getStackValue(0));
		case 430:
			if(this.maNFCWriteMifareBlocks == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCWriteMifareBlocks(i1, i2, i3, this.getStackValue(0));
		case 431:
			if(this.maNFCReadMifarePages == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCReadMifarePages(i1, i2, i3, this.getStackValue(0));
		case 432:
			if(this.maNFCWriteMifarePages == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maNFCWriteMifarePages(i1, i2, i3, this.getStackValue(0));
		case 433:
			if(this.maSyscallPanicsEnable == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSyscallPanicsEnable();
		case 434:
			if(this.maSyscallPanicsDisable == undefined)
				return MoSyncConstants.IOCTL_UNAVAILABLE;
			return this.maSyscallPanicsDisable();
	};
};
