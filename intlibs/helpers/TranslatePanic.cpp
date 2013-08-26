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
