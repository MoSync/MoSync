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
