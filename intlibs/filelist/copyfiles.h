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

#ifndef COPYFILES_H
#define COPYFILES_H

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_COPY_SRC_COULD_NOT_BE_OPENED -1
#define ERR_COPY_DST_COULD_NOT_BE_OPENED -2
#define ERR_COPY_READ_FAILED -3
#define ERR_COPY_WRITE_FAILED -4

// copies a single file
// returns < 0 on error
int copySingleFile(const char *src, const char* dst);

// copies files recursively
// returns < 0 on error
int copyFilesRecursively(const char* srddir, const char* dstdir);

#ifdef __cplusplus
}
#endif

#endif	//COPYFILES_H
