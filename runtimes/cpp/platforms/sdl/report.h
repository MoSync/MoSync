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

//Functions for sending data to the parent process

#include "report_codes.h"

void reportSetFd(int fd);
void report(int opcode, const void* buf=NULL, int count=0);
void reportClose();
void reportIp(int r, const char* message);

#define REPORT(string) { static const char s[] = string; LOG("%s\n", s);\
	report(REPORT_STRING, s, sizeof(s)-1); }
