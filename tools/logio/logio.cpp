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
#endif	//WIN32

#ifdef _MSC_VER
#include <crtdbg.h>
#define assert _ASSERTE
#else
#include <assert.h>
#endif

#define CONFIG_H	//hack
#include <helpers/helpers.h>
#include <helpers/log.h>

#ifdef _WIN32
#include <helpers/platforms/windows/HandleCloser.h>
#endif

#define BUFSIZE 1024

#ifdef _WIN32

#define DO_GLE int gle = GetLastError();\
	LOG("Error running program: %i/%i\n", gle, __LINE__); return gle
#define W32TEST(func) { int _res = (func); if(!_res) { DO_GLE; } }

static HandleCloser hReadPipe, hWritePipe;
static HandleCloser hStdinPipeWrite, hStdinPipeRead;
static HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
static HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
static HandleCloser hLogfile, hConsoleOutput;

static DWORD WINAPI OutThreadFunc(LPVOID) {
	DWORD nbr, nbw;
	char outBuf[BUFSIZE];
	while(1) {
		//read from inferior's stdout
		W32TEST(ReadFile(hReadPipe, outBuf, sizeof(outBuf), &nbr, NULL));
		//write to stdout
		W32TEST(WriteFile(hStdout, outBuf, nbr, &nbw, NULL));
		assert(nbw == nbr);
		//write to logfile
		const char so[] = "stdout: ";
		W32TEST(WriteFile(hLogfile, so, sizeof(so)-1, &nbw, NULL));
		W32TEST(WriteFile(hLogfile, outBuf, nbr, &nbw, NULL));
		assert(nbw == nbr);
#if 0
		//write to console
		W32TEST(WriteFile(hConsoleOutput, so, sizeof(so)-1, &nbw, NULL));
		W32TEST(WriteFile(hConsoleOutput, outBuf, nbr, &nbw, NULL));
		assert(nbw == nbr);
#endif
	}
}
static DWORD WINAPI InThreadFunc(LPVOID) {
	DWORD nbr, nbw;
	char inBuf[BUFSIZE];
	while(1) {
		//read from stdin
		W32TEST(ReadFile(hStdin, inBuf, sizeof(inBuf), &nbr, NULL));
		//write to inferior's stdin
		W32TEST(WriteFile(hStdinPipeWrite, inBuf, nbr, &nbw, NULL));
		assert(nbw == nbr);
		//write to logfile
		const char si[] = "stdin: ";
		W32TEST(WriteFile(hLogfile, si, sizeof(si)-1, &nbw, NULL));
		W32TEST(WriteFile(hLogfile, inBuf, nbr, &nbw, NULL));
		assert(nbw == nbr);
#if 0
		//write to console
#endif
	}
}
#endif

int exec(const char* logfilename, const char* cmdline) {
#ifdef _WIN32

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	//HandleCloser hEvent;
	SECURITY_ATTRIBUTES sa;

	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = true;
	W32TEST(CreatePipe(hReadPipe.p(), hWritePipe.p(), &sa, 0));
	W32TEST(CreatePipe(hStdinPipeRead.p(), hStdinPipeWrite.p(), &sa, 0));
	hLogfile = CreateFile(logfilename, GENERIC_WRITE, FILE_SHARE_READ, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	W32TEST(hLogfile != INVALID_HANDLE_VALUE);

	//todo: create console

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

	DWORD exitCode;

	hWritePipe.close();
	hStdinPipeRead.close();

	HandleCloser hOutThread = CreateThread(NULL, 0, OutThreadFunc, NULL, 0, NULL);
	HandleCloser hInThread = CreateThread(NULL, 0, InThreadFunc, NULL, 0, NULL);

	if(WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_FAILED) {
		DO_GLE;
	}
	W32TEST(GetExitCodeProcess(pi.hProcess, &exitCode));

	return exitCode;
#else
#define FAIL_ERRNO LOG("Errno %i @ %s:%i\n", errno, __FILE__, __LINE__); return -2
#define ERRNO(a) if((a) < 0) { FAIL_ERRNO; }
	
	LOG("execDoublePipe(%s, %i)\n", cmdline, in.size());
	
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

int main(int argc, char** argv) {
	if(argc != 3) {
		printf("Usage: logio <logfilename> <command line>\n");
		return 1;
	}
	exec(argv[1], argv[2]);
	return 0;
}
