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

#define SYMBIAN_ERRORS(m)\
	m(200001, SYMERR_BAD_COMMAND_LINE, "Bad command line")\
	m(200002, SYMERR_EVENT_BUFFER_FULL, "Event buffer full")\
	m(200003, SYMERR_IMAGE_LOAD_FAILED, "Failed to load image")\
	m(200004, SYMERR_VIBRATION_DURATION_NEGATIVE, "Negative vibration duration")\
	m(200005, SYMERR_NO_MATCHING_DECODER, "No matching decoder found")\
	m(200006, SYMERR_SOUND_DECODING_ERROR, "Error while decoding sound"/* Can happen when MIME type and sound file don't match. */ )\

DECLARE_ERROR_ENUM(SYMBIAN)
