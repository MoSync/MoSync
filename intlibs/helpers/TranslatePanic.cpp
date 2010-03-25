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

//hack
#define CONFIG_H
#define LOGGING_ENABLED

#include "helpers.h"
#include <bluetooth/bt_errors.h>
#include "../../runtimes/cpp/base/base_errors.h"
#include "../../runtimes/cpp/platforms/sdl/sdl_errors.h"
#include "../../runtimes/cpp/platforms/sdl/windows_errors.h"
#include "../../runtimes/cpp/platforms/symbian/inc/symbian_errors.h"
#include "../../runtimes/cpp/platforms/winmobile/wce_errors.h"

#define ERROR_SETS(m) m(HELPERS) m(BASE) m(SDL) m(BT) m(WINDOWS) m(SYMBIAN) m(WCE)

#include "errors_inline.h"

using namespace MoSyncError;

const char* panicDescription(int code) {
	return errorDescription(code);
}

const char* panicName(int code) {
#define CASE_PANIC(id, name, desc) case name: return #name;
#define CASE_SET(set) set##_ERRORS(CASE_PANIC)

	switch(code) {
		ERROR_SETS(CASE_SET);
	default:
		return NULL;
	}
}
