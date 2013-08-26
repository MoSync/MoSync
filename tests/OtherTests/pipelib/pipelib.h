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

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PIPELIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PIPELIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PIPELIB_EXPORTS
#define PIPELIB_API __declspec(dllexport)
#else
#define PIPELIB_API __declspec(dllimport)
#endif

extern "C" PIPELIB_API int pipe_create(int* fds);

extern "C" PIPELIB_API int pipe_read(int fd, void* dstbuf, int maxchars);

extern "C" PIPELIB_API int pipe_write(int fd, void* srcbuf, int length);

extern "C" PIPELIB_API int pipe_dup(int fd);

extern "C" PIPELIB_API int pipe_dup2(int fd1, int fd2);

extern "C" PIPELIB_API int pipe_close(int fd);

extern "C" PIPELIB_API int proc_spawn(char* cmd, char* args, char* dir);

extern "C" PIPELIB_API int proc_wait_for(int handle);

extern "C" PIPELIB_API int proc_kill(int pid, int exit_code);
