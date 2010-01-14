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

#define SDL_ERRORS(m)\
	m(80001, SDLERR_SOUND_LOOP_FAILED, "Failed to loop sound")\
	m(80002, SDLERR_SOUND_LOAD_FAILED, "Failed to load sound")\
	m(80003, SDLERR_MOSYNCDIR_NOT_FOUND, "Environment variable MOSYNCDIR could not be found")\
	m(80004, SDLERR_IMAGE_LOAD_FAILED, "Failed to load image")\
	m(80005, SDLERR_SPRITE_LOAD_FAILED, "Failed to load sprite")\
	m(80007, SDLERR_SCREEN_NOT_32BIT, "The screen is not set to 32-bit color depth")\
	m(80008, SDLERR_UNDEFINED_KEYCODE, "Undefined keycode")\
	m(80009, SDLERR_TEXT_SIZE_FAILED, "Failed to calculate text size")\
	m(80010, SDLERR_TEXT_RENDER_FAILED, "Failed to render text")\
	m(80011, SDLERR_SOUND_DECODE_FAILED, "Failed to decode sound")\
	m(80012, SDLERR_NOSKIN, "Selected skin unavailable")\

DECLARE_ERROR_ENUM(SDL);
