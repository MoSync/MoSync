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

#define WCE_ERRORS(m)\
	m(240001, WCEERR_SOUND_LOOP_FAILED, "Failed to loop sound")\
	m(240002, WCEERR_SOUND_LOAD_FAILED, "Failed to load sound")\
	m(240003, WCEERR_SOUND_MIME_CORRUPTED, "Corrupted mime string for sound media")\
	m(240004, WCEERR_SOUND_UNKNOWN_MIME, "Unknown mime type for sound media")\
	m(240005, WCEERR_IMAGE_LOAD_FAILED, "Failed to load image")\
	m(240006, WCEERR_SPRITE_LOAD_FAILED, "Failed to load sprite")\
	m(240008, WCEERR_UNDEFINED_KEYCODE, "Undefined keycode")\
	m(240009, WCEERR_TEXT_SIZE_FAILED, "Failed to calculate text size")\
	m(240010, WCEERR_TEXT_RENDER_FAILED, "Failed to render text")\
	m(240011, WCEERR_TIMEZONE, "Could not retrieve time zone information")\
	m(240012, WCEERR_UNSUPPORTED_BPP, "Unsupported pixel color depth")\
	m(240013, WCEERR_SOURCE_RECT_OOB, "Source rect out of bounds")\
	m(240014, WCEERR_STORE_FILE_NOT_FOUND, "Store file not found")\
	m(240015, WCEERR_STORE_ACCESS_DENIED, "Store file access denied")\
	m(240016, WCEERR_STORE_CLOSE_FAILED, "Store close failed")\
	m(240017, WCEERR_UNSUPPORTED_OS_VERSION, "Unsupported OS version")\
	m(240018, WCEERR_COULD_NOT_CREATE_EVENT, "Could not create windows event.")

DECLARE_ERROR_ENUM(WCE);
