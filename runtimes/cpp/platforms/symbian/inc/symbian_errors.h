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

#define SYMBIAN_ERRORS(m)\
	m(200001, SYMERR_BAD_COMMAND_LINE, "Bad command line")\
	m(200002, SYMERR_EVENT_BUFFER_FULL, "Event buffer full")\
	m(200003, SYMERR_IMAGE_LOAD_FAILED, "Failed to load image")\
	m(200004, SYMERR_VIBRATION_DURATION_NEGATIVE, "Negative vibration duration")\
	m(200005, SYMERR_NO_MATCHING_DECODER, "No matching decoder found")\
	m(200006, SYMERR_SOUND_DECODING_ERROR, "Error while decoding sound"/* Can happen when MIME type and sound file don't match. */ )\
	m(200007, SYMERR_IAP_METHOD, "Invalid IAP method")\
	m(200008, SYMERR_IAP_FILTER, "Invalid IAP filter")\

DECLARE_ERROR_ENUM(SYMBIAN)
