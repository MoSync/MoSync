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

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

//hack
#define CONFIG_H
#define LOGGING_ENABLED

#include <helpers/log.h>
#include <helpers/helpers.h>

using namespace MoSyncError;

void InitLog(const char*) {
}

void LogBin(const void* data, int size) {
	int res;
	res = fwrite(data, 1, size, stdout);
	if(res != size)
		MoSyncErrorExit(ERR_INTERNAL);
}

void LogV(const char* fmt, va_list args) {
	int res;
	res = vfprintf(stdout, fmt, args);
	if(res < 0)
		MoSyncErrorExit(ERR_INTERNAL);
}

void LogTime(const char* fmt, ...) {
}

void failFunction() {
}
