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
