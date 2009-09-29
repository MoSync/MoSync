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

#include <stddef.h>
#include <string.h>

extern "C" {
//#ifndef _STRING_H_
//const char* strrchr(const char* str, int c);
//#endif
size_t strlen(const char* str);
char* strcpy(char* dst, const char* src);
int strcmp(const char* a, const char* b);
int strncmp(const char *string1, const char *string2, size_t count);
int isdigit(int c);

int memcmp(const void* a, const void* b, size_t len);
void* memcpy(void* dst, const void* src, size_t len);
}
