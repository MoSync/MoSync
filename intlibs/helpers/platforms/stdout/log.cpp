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
