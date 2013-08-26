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
}
