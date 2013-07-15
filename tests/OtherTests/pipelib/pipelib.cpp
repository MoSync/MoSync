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

// pipelib.cpp : Defines the exported functions for the DLL application.
//

//#include "stdafx.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "pipelib.h"

#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <process.h>
#include <direct.h>

PIPELIB_API int pipe_create(int* fds)
{
	return _pipe(fds, 512, O_BINARY/* | O_NOINHERIT*/);
}

PIPELIB_API int pipe_read(int fd, void* dstbuf, int maxchars) {
	return _read(fd, dstbuf, maxchars);
}

PIPELIB_API int pipe_write(int fd, void* srcbuf, int length) {
	return _write(fd, srcbuf, length);
}

PIPELIB_API int pipe_dup(int fd) {
	return _dup(fd);
}

PIPELIB_API int pipe_dup2(int fd1, int fd2) {
	return _dup2(fd1, fd2);
}

PIPELIB_API int pipe_close(int fd) {
	return _close(fd);
}

PIPELIB_API int proc_spawn(char* cmd, char* args, char* dir) {
	if(_chdir(dir) != 0)
		return -1;
	return (int)_spawnl(_P_NOWAIT, cmd, args, NULL);
}

PIPELIB_API int proc_wait_for(int handle) {
	int err_code;
	_cwait(&err_code, handle, NULL);
	return err_code;
}

PIPELIB_API int proc_kill(int pid, int exit_code) {
	HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (handle != NULL) {
		return TerminateProcess(handle, exit_code);
	} else {
		return -1;
	}
}
