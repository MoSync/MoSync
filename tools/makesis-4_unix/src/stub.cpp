#include "utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> /* mbstowcs */
#include <unistd.h> /* read */
#include <errno.h>
#include <stdio.h>
#include <wchar.h> /* wcslen */
#include <string.h> /* strncpy */
#include <stdint.h>


#ifdef UNICODE
#error "not supported yet..."
#endif


UINT GetTempPath(UINT maxlen, LPSTR ptr)
{
	strncpy(ptr, "/tmp", maxlen);
	return 0;
}


BOOL CreateDirectory(LPCSTR pathname, void*)
{
	const int ret = mkdir(pathname, 0700);
	if(ret == -1) {
	  if(errno != EEXIST)
	    printf("CreateDirectory: %s: %s\n", pathname, strerror(errno));
	  return FALSE;
	}
	return TRUE;
}


HANDLE CreateFileA(LPCSTR filename, DWORD access, DWORD sharing,
		   void*, DWORD creation,
		   DWORD attributes, HANDLE)
{
	(void)attributes;

	int fd = -1;

	if(creation == OPEN_EXISTING) {
		fd = open(filename, O_RDONLY);
	}
	else if(creation == CREATE_NEW) {
		fd = creat(filename, 0664);
	}
	else if(creation == CREATE_ALWAYS) {
		fd = creat(filename, 0664);
	}
	else {
		printf("CreateFileA: unknown creation flag %lu\n", creation);
	}

	if(fd < 0) {
		if(errno != ENOENT)
			printf("CreateFileA: '%s' %s\n", filename, strerror(errno));
		return INVALID_HANDLE_VALUE;
	}
	else
		return (void *)fd;
}


BOOL ReadFile(HANDLE hFile, LPVOID buffer, DWORD bytesToRead,
	      LPDWORD bytesRead, void *)
{
	int fd = (intptr_t)hFile;

	const int size = read(fd, buffer, bytesToRead);

	if(size < 0)
		return FALSE;

	if(bytesRead)
		*bytesRead = size;

	return TRUE;
}


BOOL CloseHandle(HANDLE fd)
{
	return close((intptr_t)fd) == 0;
}


DWORD WINAPI GetFileSize(HANDLE h, LPDWORD)
{
	int fd = (intptr_t)h;
	int ret = 0;

	struct stat s;

	ret = fstat(fd, &s);

	if(ret != 0) {
		printf("GetFileSize: %s\n", strerror(errno));
		return 0;
	}
  
	return s.st_size;
}


//
// unicode stuff
//


INT WINAPI MultiByteToWideChar(UINT page,
			DWORD flags,
			LPCSTR src, INT srclen,
			LPWSTR dst, INT dstlen)
{
  (void)page;
  (void)flags;
  (void)srclen;

  return mbstowcs(dst, src, dstlen);
}

INT WINAPI WideCharToMultiByte(UINT page,
			       DWORD flags,
			       LPCWSTR src, INT srclen,
			       LPSTR dst, INT dstlen,
			       LPCSTR defchar,
			       BOOL *used)
{
  (void)page;
  (void)flags;
  (void)srclen;
  (void)defchar;
  (void)used;

  return wcstombs(dst, src, dstlen);
}


int _wunlink(const wchar_t* wc)
{
  int ret = 0;
  int len = wcslen(wc);

  char *tmp = new char[len+1];
  ret = wcstombs(tmp, wc, len);
  if(ret == -1) {
    printf("wunlink: wcstombs error\n");
    goto out;
  }
  tmp[ret] = '\0';
  ret = unlink(tmp);
  if(ret != 0)
    printf("wunlink: %s: %s\n", tmp, strerror(errno));

 out:
  delete[] tmp;

  return ret;
}


BOOL WriteFile(HANDLE hFile,
	       LPCVOID buffer, DWORD bytesToWrite,
	       LPDWORD bytesWritten, void*)
{
	int fd = (intptr_t)hFile;

	ssize_t ret = write(fd, buffer, bytesToWrite);
	if(ret == -1) {
		printf("WriteFile: %s", strerror(errno));
		return FALSE;
	}
	if(bytesWritten)
		*bytesWritten = ret;

	return TRUE;
}


DWORD SetFilePointer(HANDLE hFile,
		     LONG distance,
		     LONG *highword,
		     DWORD method)
{
  (void)highword;

  int fd = (intptr_t)hFile;
  off_t off = 0;

  if(method == FILE_BEGIN)
    off = lseek(fd, (off_t)distance, SEEK_SET);
  else if(method == FILE_CURRENT)
    off = lseek(fd, (off_t)distance, SEEK_CUR);
  else {
    printf("### NOT IMPLEMENTED ### %s\n", __FUNCTION__);
  }

  return off;
}


BOOL RemoveDirectory(LPCSTR a)
{
	const int ret = rmdir(a);
	if(ret != 0)
		printf("RemoveDirectory: '%s' %s\n", a, strerror(errno));
	return(ret == 0);
}


//
// TODO:
//


char* _fullpath(char*,const char*, size_t)
{
  printf("### NOT IMPLEMENTED ### %s\n", __FUNCTION__);
  return 0;
}
