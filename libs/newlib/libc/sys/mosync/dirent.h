/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/*
This implementation does not support XSI extensions such as
dirent::d_ino, seekdir() and telldir().
*/

#ifndef _DIRENT_H_
#define _DIRENT_H_
#ifdef __cplusplus
extern "C" {
#endif

#define MAXNAMLEN 384
#define NAME_MAX MAXNAMLEN

#define _DIRENT_HAVE_D_NAMLEN 1
#define _DIRENT_HAVE_D_TYPE 1

#define DT_DIR S_IFDIR	// Directory
#define DT_REG S_IFREG	// Regular file

typedef struct dirent {
	int d_type;	// one of the DT_ constants.
	int d_namlen;	// length of name, excluding the terminating zero.
	char d_name[MAXNAMLEN];
} dirent;

typedef struct DIR {
	int dd_fd;	/* directory file */
	char *dd_buf;	/* buffer */
	int dd_len;	/* buffer length */
//	int dd_size;	/* amount of data in buffer */
} DIR;

int alphasort(const dirent ** d1, const dirent ** d2);
int closedir(DIR*);
int dirfd(DIR*);
DIR* fdopendir(int fd);
DIR* opendir(const char* name);
dirent* readdir(DIR*);
int readdir_r(DIR* restrict, dirent* __restrict entry, dirent ** __restrict result);
void rewinddir(DIR*);
int scandir(const char* name, dirent *** namelist,
	int (*sel)(const dirent *),
	int (*compare)(const dirent **, const dirent **));

int getdents(int fd, dirent* dp, int count);

#ifdef __cplusplus
}
#endif
#endif /*_DIRENT_H_*/
