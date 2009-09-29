/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#define WCE_ERRORS(m)\
	m(WCEERR_SOUND_LOOP_FAILED, "Failed to loop sound")\
	m(WCEERR_SOUND_LOAD_FAILED, "Failed to load sound")\
	m(WCEERR_SOUND_MIME_CORRUPTED, "Corrupted mime string for sound media")\
	m(WCEERR_SOUND_UNKNOWN_MIME, "Unknown mime type for sound media")\
	m(WCEERR_IMAGE_LOAD_FAILED, "Failed to load image")\
	m(WCEERR_SPRITE_LOAD_FAILED, "Failed to load sprite")\
	m(WCEERR_TILESET_LOAD_FAILED, "Failed to load tileset")\
	m(WCEERR_UNDEFINED_KEYCODE, "Undefined keycode")\
	m(WCEERR_TEXT_SIZE_FAILED, "Failed to calculate text size")\
	m(WCEERR_TEXT_RENDER_FAILED, "Failed to render text")\
	m(WCEERR_TIMEZONE, "Could not retrieve time zone information")\
	m(WCEERR_UNSUPPORTED_BPP, "Unsupported pixel color depth")\
	m(WCEERR_SOURCE_RECT_OOB, "Source rect out of bounds")\
	m(WCEERR_STORE_FILE_NOT_FOUND, "Store file not found")\
	m(WCEERR_STORE_ACCESS_DENIED, "Store file access denied")\
	m(WCEERR_STORE_CLOSE_FAILED, "Store close failed")\
	m(WCEERR_UNSUPPORTED_OS_VERSION, "Unsupported OS version")\

DECLARE_ERROR_ENUM(WCE);
