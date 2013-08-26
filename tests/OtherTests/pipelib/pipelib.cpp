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
