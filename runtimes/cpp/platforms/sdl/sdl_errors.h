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
	m(SDLERR_SOUND_LOOP_FAILED, "Failed to loop sound")\
	m(SDLERR_SOUND_LOAD_FAILED, "Failed to load sound")\
	m(SDLERR_MOSYNCDIR_NOT_FOUND, "Environment variable MOSYNCDIR could not be found")\
	m(SDLERR_IMAGE_LOAD_FAILED, "Failed to load image")\
	m(SDLERR_SPRITE_LOAD_FAILED, "Failed to load sprite")\
	m(SDLERR_SCREEN_NOT_32BIT, "The screen is not set to 32-bit color depth")\
	m(SDLERR_UNDEFINED_KEYCODE, "Undefined keycode")\
	m(SDLERR_TEXT_SIZE_FAILED, "Failed to calculate text size")\
	m(SDLERR_TEXT_RENDER_FAILED, "Failed to render text")\
	m(SDLERR_SOUND_DECODE_FAILED, "Failed to decode sound")\
	m(SDLERR_NOSKIN, "Selected skin unavailable")\

DECLARE_ERROR_ENUM(SDL);
