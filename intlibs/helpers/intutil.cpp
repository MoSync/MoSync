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
#define _WIN32_WINNT 0x0500
#include <windows.h>
#else
#include <errno.h>
#include <sys/wait.h>
#endif

#include "intutil.h"

#ifdef PTAH_EXPORTS
#define CONFIG_H	//hack
#else
#include <Platform.h>
#endif

#include <helpers/helpers.h>
#include <helpers/log.h>

#ifdef _WIN32
#include <helpers/platforms/windows/HandleCloser.h>
#endif

//originally copied from Ptah::BuildThread::execute().

#define BUFSIZE 1024

int execDoublePipe(const char* cmdline, const std::string& in, std::string& out) {
#ifdef _WIN32

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HandleCloser hReadPipe, hWritePipe;
	HandleCloser hStdinPipeWrite, hStdinPipeRead;
	HandleCloser hEvent;
	SECURITY_ATTRIBUTES sa;

#define DO_GLE int gle = GetLastError();\
	LOG("Error running program: %i/%i\n", gle, __LINE__); return gle
#define W32TEST(func) { int _res = (func); if(!_res) { DO_GLE; } }

	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = true;
	W32TEST(CreatePipe(hReadPipe.p(), hWritePipe.p(), &sa, 0));
	W32TEST(hStdinPipeWrite = CreateNamedPipe("\\\\.\\pipe\\MoSyncStdinPipe",
		PIPE_ACCESS_OUTBOUND | FILE_FLAG_FIRST_PIPE_INSTANCE | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE | PIPE_WAIT, 1, BUFSIZE, BUFSIZE, 5000, NULL));
	W32TEST(hStdinPipeRead = CreateFile("\\\\.\\pipe\\MoSyncStdinPipe", GENERIC_READ,
		FILE_SHARE_READ, &sa, OPEN_EXISTING, 0, NULL));
   
	ZERO_OBJECT(si);
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = hWritePipe;
	si.hStdError = hWritePipe;
	si.hStdInput = hStdinPipeRead;

	std::string commandLine = cmdline;

	//will modify string. is ok since commandLine is a copy.
	W32TEST(CreateProcess(NULL, (char*)commandLine.c_str(), NULL, NULL, true,
		CREATE_NO_WINDOW, NULL, NULL, &si, &pi));

	HandleCloser hcProcess = pi.hProcess;
	HandleCloser hcThread = pi.hThread;

	int res;
	DWORD exitCode;

	hWritePipe.close();
	hStdinPipeRead.close();

	//dangerous; might deadlock if too much data is written; the stdout buffer fills up,
	//and we won't start reading from it until we've written everything.
	DWORD nbw;
	OVERLAPPED ol;
	ol.hEvent = hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	W32TEST((int)ol.hEvent);
	ol.Offset = ol.OffsetHigh = 0;
	res = WriteFile(hStdinPipeWrite, in.c_str(), in.length(), &nbw, &ol);
	if(res == 0) {
		if(GetLastError() != ERROR_IO_PENDING) {
			DO_GLE;
		}
	} else {
		if(nbw != in.length()) {
			LOG("WriteFile mismatch: %i!=%i\n", (int)nbw, (int)in.length());
			return -1;
		}
		DEBUG_ASSERT(SetEvent(hEvent));
	}

	out.clear();

	while(1) {
		char buffer[BUFSIZE];
		DWORD nBytesRead;
#if 0
		W32TEST(ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &nBytesRead, NULL));
#else
		res = ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &nBytesRead, NULL);
		if(!res) {
			if(GetLastError() != ERROR_BROKEN_PIPE) {
				DO_GLE;
			}
			break;
		}
#endif
		buffer[nBytesRead] = 0;
		out += buffer;

		if(hStdinPipeWrite != 0) {
			res = WaitForSingleObject(hEvent, 0);
			if(res == WAIT_OBJECT_0) {
				hStdinPipeWrite.close();
			} else if(res != WAIT_TIMEOUT) {
				LOG("WaitForSingleObject error %i\n", res);
				DO_GLE;
			}
		}
	}

	if(WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_FAILED) {
		DO_GLE;
	}
	W32TEST(GetExitCodeProcess(pi.hProcess, &exitCode));

	return exitCode;
#else
#define FAIL_ERRNO LOG("Errno %i @ %s:%i\n", errno, __FILE__, __LINE__); return -2
#define ERRNO(a) if((a) < 0) { FAIL_ERRNO; }
	
	LOG("execDoublePipe(%s, %lu)\n", cmdline, in.size());
	
	int pin[2], pout[2];
	ERRNO(pipe(pin));
	ERRNO(pipe(pout));
	
	pid_t pid = fork();
	if(pid == 0) {	//child
		if(dup2(pin[0], STDIN_FILENO) < 0)
			_exit(errno);
		close(pin[1]);
		if(dup2(pout[1], STDOUT_FILENO) < 0)
			_exit(errno);
		close(pout[0]);
		execvp(cmdline, NULL);	//if this function returns, it has failed.
		_exit(errno);
	} else {	//parent
		if(pid < 0) {	//fork failed
			FAIL_ERRNO;
		}
	}

	ERRNO(close(pin[0]));
	FILE* fout = fdopen(pin[1], "w");
	if(!fout) {
		FAIL_ERRNO;
	}
	ERRNO(close(pout[1]));

	fprintf(fout, "%s", in.c_str());
	ERRNO(fclose(fout));

	out.clear();
	while(1) {
		char buffer[BUFSIZE];
		ssize_t res;
		ERRNO(res = read(pout[0], buffer, BUFSIZE));
		if(res == 0)	//EOF
			break;
		out.append(buffer, res);
	}

	ERRNO(close(pout[0]));

	int status;
	ERRNO(waitpid(pid, &status, 0));
	errno = status;
	ERRNO(errno);
	return status;
#endif
}
