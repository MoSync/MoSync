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

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#define _write write
#define _close close
#endif

#include "config_platform.h"
#include <helpers/helpers.h>
#include "report.h"

static int sFd = 0;

void reportSetFd(int fd) {
	sFd = fd;
}

void report(int opcode, const void* buf, int count) {
	if(sFd == 0)
		return;
	DEBUG_ASSERT(_write(sFd, &opcode, sizeof(int)) == sizeof(int));
	DEBUG_ASSERT(_write(sFd, &count, sizeof(int)) == sizeof(int));
	if(count) {
		DEBUG_ASSERT(_write(sFd, buf, count) == count);
	}
}

void reportClose() {
	if(sFd == 0)
		return;
	DEBUG_ASSERT(_close(sFd) == 0);
	sFd = 0;
}
