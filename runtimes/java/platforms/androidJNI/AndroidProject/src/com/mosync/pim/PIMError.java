package com.mosync.pim;

public class PIMError {

	static final int PANIC_LIST_TYPE_INVALID = 40071;
	static final int PANIC_LIST_UNAVAILABLE = 40072;
	static final int PANIC_LIST_ALREADY_OPENED = 40073;
	static final int PANIC_FIELD_INVALID = 40074;
	static final int PANIC_FIELD_EMPTY = 40075;
	static final int PANIC_INDEX_INVALID = 40076;
	static final int PANIC_HANDLE_INVALID = 40077;
	static final int PANIC_BUFFER_INVALID = 40078;

	static String sStrListTypeInvalid = "Invalid list type.";
	static String sStrListUnavailable = "List is not available.";
	static String sStrListAlreadyOpened = "List already opened.";
	static String sStrFieldInvalid = "Invalid field.";
	static String sStrFieldEmpty = "Empty field.";
	static String sStrIndexInvalid = "Invalid index.";
	static String sStrHandleInvalid = "Invalid handle.";
	static String sStrBufferInvalid = "Invalid buffer structure.";
	// /// The operation is not allowed by the Address Book database.
	// OPERATION_NOT_PERMITTED = -20;
}
